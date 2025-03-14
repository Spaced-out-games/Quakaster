#pragma once
#include "InputEvents.h"
#include <include/base/Event.h>
#include <include/base/System.h>
#include <include/server/interpreter/ConsoleInterpreter.h>
#include <include/UI/UIContext.h>
#include <include/utils/utility_types.h>
#include <include/IO/InputEvents.h>
#include <unordered_map>
#include <include/common.h>




struct IOState {
	glm::ivec2 last_mouse;
	glm::ivec2 rel_mouse;
	std::unordered_map<int, bool> key_states;
};

template <typename event_t>
struct QKEventListener {
	std::function<void(event_t)> on_event;

	QKEventListener(std::function<void(event_t)> on_event = [](event_t) {}) : on_event(on_event)
	{
		QKEvent::dispatcher.sink<event_t>().connect<&QKEventListener::onEvent>(this);
	}

private:
	void onEvent(event_t evt) {
		on_event(evt);
	}
};


// Fires input events that controllers and the like can hook onto
struct InputDelegate : ISystem {

	bool isImGuiInitialized() const { return ImGui::GetCurrentContext() != nullptr; }
	void init(QKScene& scene) override;
	void tick(QKScene& scene) override;
	void destroy(QKScene& scene) override {};
	inline void togglePause() { m_IsPaused ^= 1; }
	inline bool& is_paused() { return m_IsPaused; }
	inline bool& was_paused() { return last_pause_state; }

	private:

		// For storing immediate SDL Events as they poll.
		SDL_Event event; // was event

		// For handling ImGUI state information
		ImGuiIO* ImGuiState = nullptr; // was ImGuiState

		// This is how we dispatch events

		IOState KeyboardMouseState; // was lastMouseX, lastMouseY, keyStates
		bool m_IsPaused = false;
		bool last_pause_state = true;

};




void InputDelegate::init(QKScene& scene) {
	ImGuiState = &ImGui::GetIO();
	SDL_SetRelativeMouseMode(is_paused() ? SDL_TRUE : SDL_FALSE); // make sure it's synced
}

void InputDelegate::tick(QKScene& scene) {

	// Make sure ImGui is ready to go, exit if not
	if (!ImGuiState) init(scene);

	// reset the relative mouse positions
	KeyboardMouseState.rel_mouse = { 0,0 };

	// start polling
	while (SDL_PollEvent(&event))
	{
		// ESC key always intercepts events, because regardless of current pause state, it should invert the pause state. Might be better to make this not hard-coded so we can actually bind custom shit
		if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE)
		{
			togglePause();
			//if (!is_paused()) last_pause_state = 0;
			SDL_SetRelativeMouseMode(is_paused() ? SDL_TRUE : SDL_FALSE);
			// Fire a pause event
			// QKEvent::fire(PauseEvent{});

			// we are done, so return
			return;
		}

		//LOG(is_paused() << '\n');

		// Forward it to ImGui
		ImGui_ImplSDL2_ProcessEvent(&event);

		// Handle whatever ImGui rejects
		if ((!ImGuiState->WantCaptureKeyboard || !ImGuiState->WantCaptureMouse) && is_paused())
		{
			switch (event.type)
			{
			case SDL_KEYDOWN:
				if (KeyboardMouseState.key_states[static_cast<int>(event.key.keysym.sym)] == 0)
				{

					QKEvent::fire<KeyPressEvent>(event.key.keysym.sym, event.key.keysym.mod);
					KeyboardMouseState.key_states[static_cast<int>(event.key.keysym.sym)] = 1;
				}
				break;
			case SDL_KEYUP:
				QKEvent::fire<KeyReleaseEvent>(event.key.keysym.sym, event.key.keysym.mod);
				// Removes the key from the hash map so it's not iterated over anymore
				KeyboardMouseState.key_states.erase(static_cast<int>(event.key.keysym.sym)); 
				break;
			case SDL_MOUSEBUTTONDOWN:
				if(event.type == SDL_MOUSEBUTTONDOWN) QKEvent::fire<MouseClickEvent>(event.button.button, event.button.x, event.button.y, true);
				break;
			case SDL_MOUSEBUTTONUP:
				QKEvent::fire<MouseClickEvent>(event.button.button, event.button.x, event.button.y, false);
				break;
			case SDL_WINDOWEVENT:
				QKEvent::fire<WindowEvent>(event.window.event);
				break;
			case SDL_MOUSEMOTION:
				// Get mouse motion details
				KeyboardMouseState.last_mouse = { event.motion.x, event.motion.y };
				KeyboardMouseState.rel_mouse = { event.motion.xrel, event.motion.yrel };
				QKEvent::fire<MouseMoveEvent>(KeyboardMouseState.last_mouse, KeyboardMouseState.rel_mouse);

				break;

			default:
				break;
			}
		}
	}


	if (!ImGuiState->WantCaptureKeyboard || !ImGuiState->WantCaptureMouse)
	{
		if (!ImGuiState->WantCaptureKeyboard || !ImGuiState->WantCaptureMouse)
		{
			for (const auto& [key, isPressed] : KeyboardMouseState.key_states)
			{
				if (isPressed)
				{
					// Trigger a repeat event for the pressed key
					QKEvent::fire<KeyHoldEvent>(key);
				}
			}
		}
	}

}
