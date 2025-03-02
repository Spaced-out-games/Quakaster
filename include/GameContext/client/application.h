#pragma once
#include <include/GameContext/window/window.h>
#include <include/GameContext/UI/UIContext.h>
#include <include/GameContext/IO/InputDelegate.h>
#include <include/GameContext/base/eventHandler.h>
#include <include/GameContext/client/Client.h>
#include <include/GameContext/base/System.h>




struct Application
{
	// Content window
	Window window;

	// GUI context
	UIContext ui_context;

	// IP address, player ID, sockets, client-side copy of the scene. also, client-side systems
	Client cl;

	// color of the background
	glm::vec3 bg_color = { 0.0f,1.0f,1.0f };


	// Timing variables using std::chrono
	std::chrono::steady_clock::time_point lastFrameTime;
	std::chrono::steady_clock::time_point currentFrameTime;

	InputDelegate input_delegate;

	// Error code or exit
	int status = 1;


	Application(EventHandler& event_handler, ConsoleInterpreter& interpreter) : input_delegate(event_handler), ui_context( event_handler, interpreter, window.get_renderer(), window, input_delegate), window(interpreter)
	{

	}

	// Updates timing information for the current frame
	inline void update_dt()
	{
		currentFrameTime = std::chrono::steady_clock::now(); // Get current time
		std::chrono::duration<float> elapsed = currentFrameTime - lastFrameTime; // Time elapsed
		deltaTime = elapsed.count(); // Convert to seconds
		lastFrameTime = currentFrameTime; // Update the last frame time
	}

	inline void exit() {
		status = 0;
	}

	static const float& get_deltaTime() { return deltaTime; }

	private:
		static inline float deltaTime = 0.0f;
};