#pragma once
#include "InputEvents.h"
#include <include/GameContext/base/eventHandler.h>
#include <include/GameContext/base/System.h>
#include <include/GameContext/server/interpreter/ConsoleInterpreter.h>
#include <include/GameContext/UI/UIContext.h>
#include <include/GameContext/utils/utility_types.h>
#include <include/GameContext/IO/InputEvents.h>
#include <unordered_map>
#include <imgui.h>


struct IOState {
	glm::ivec2 last_mouse;
	glm::ivec2 rel_mouse;
	std::unordered_map<int, bool> key_states;
};

template <typename event_t>
struct EventListener {
	std::function<void(event_t)> on_event;

	EventListener(EventHandler& dispatcher, std::function<void(event_t)> on_event = [](event_t) {}) : on_event(on_event)
	{
		dispatcher.sink<event_t>().connect<&EventListener::onEvent>(this);
	}

private:
	void onEvent(event_t evt) {
		on_event(evt);
	}
};


// Fires input events that controllers and the like can hook onto
struct InputDelegate : ISystem {
	InputDelegate(EventHandler& handler) : event_handler(handler) {}
	// For storing immediate SDL Events as they poll.
	SDL_Event event; // was event

	// For handling ImGUI state information
	ImGuiIO* ImGuiState = nullptr; // was ImGuiState

	// This is how we dispatch events
	EventHandler& event_handler; // was dispatcher

	IOState KeyboardMouseState; // was lastMouseX, lastMouseY, keyStates
	bool is_paused = false;
	bool last_pause_state = false;

	bool isImGuiInitialized() const { return ImGui::GetCurrentContext() != nullptr; }
	void init(Scene& scene) override;
	void tick(Scene& scene) override;
	void destroy(Scene& scene) override {};
};




void InputDelegate::init(Scene& scene) {
	ImGuiState = &ImGui::GetIO();
}

void InputDelegate::tick(Scene& scene) {

	// Make sure ImGui is ready to go, exit if not
	if (!ImGuiState) init(scene);

	// reset the relative mouse positions
	KeyboardMouseState.rel_mouse = { 0,0 };

	// start polling
	while (SDL_PollEvent(&event))
	{
		// ESC key always intercepts events, because regardless of pause state, it should invert the pause state
		if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE)
		{
			is_paused = !is_paused;
			if (!is_paused) last_pause_state = 0;
			SDL_SetRelativeMouseMode(is_paused ? SDL_TRUE : SDL_FALSE);
			// Fire a pause event
			//event_handler.trigger(PauseEvent{});

			// we are done, so return
			return;
		}

		std::cout << is_paused << '\n';

		// Forward it to ImGui
		ImGui_ImplSDL2_ProcessEvent(&event);

		// Handle whatever ImGui rejects
		if ((!ImGuiState->WantCaptureKeyboard || !ImGuiState->WantCaptureMouse) && is_paused)
		{
			switch (event.type)
			{
			case SDL_KEYDOWN:
				event_handler.trigger(KeyPressEvent{ event.key.keysym.sym, event.key.keysym.mod });
				KeyboardMouseState.key_states[static_cast<int>(event.key.keysym.sym)] = 1;
				break;
			case SDL_KEYUP:
				event_handler.trigger(KeyReleaseEvent{ event.key.keysym.sym, event.key.keysym.mod });
				// Removes the key from the hash map so it's not iterated over anymore
				KeyboardMouseState.key_states.erase(static_cast<int>(event.key.keysym.sym)); 
				break;
			case SDL_MOUSEBUTTONDOWN:
				event_handler.trigger(MouseClickEvent{ event.button.button, event.button.x, event.button.y, true });
				break;
			case SDL_MOUSEBUTTONUP:
				event_handler.trigger(MouseClickEvent{ event.button.button, event.button.x, event.button.y, false });
				break;
			case SDL_WINDOWEVENT:
				event_handler.trigger(WindowEvent{ event.window.event });
				break;
			case SDL_MOUSEMOTION:
				// Get mouse motion details
				KeyboardMouseState.last_mouse = { event.motion.x, event.motion.y };
				KeyboardMouseState.rel_mouse = { event.motion.xrel, event.motion.yrel };
				event_handler.trigger(MouseMoveEvent{ KeyboardMouseState.last_mouse, KeyboardMouseState.rel_mouse });

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
					event_handler.trigger(KeyHoldEvent{ key });
				}
			}
		}
	}

}
