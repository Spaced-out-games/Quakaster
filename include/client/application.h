#pragma once
#include <include/window/window.h>
#include <include/UI/UIContext.h>
#include <include/IO/InputDelegate.h>
#include <include/base/eventHandler.h>
#include <include/client/Client.h>
#include <include/base/System.h>




struct Application
{
public:
	/// <summary>
	/// Runs the application
	/// </summary>
	/// <returns>the status code upon exit</returns>
	virtual int run() = 0;

	/// <summary>
	/// Gets the time since the last frame
	/// </summary>
	/// <returns></returns>
	static const float& get_deltaTime() { return deltaTime; }
	static const float& get_elapsedTime() { return elapsedTime; }

protected:
	// Content window
	Window window;

	// IP address, player ID, sockets, client-side copy of the scene. also, client-side systems
	Client cl;

	// color of the background
	glm::vec3 bg_color = { 0.0f,1.0f,1.0f };


	// Timing variables using std::chrono
	std::chrono::steady_clock::time_point lastFrameTime;
	std::chrono::steady_clock::time_point currentFrameTime;


	// Error code or exit
	int status = 1;

	// Systems to run the simulation
	//std::vector<ISystem*> systems;

	std::vector<Entity*> entities;


	static inline EventHandler event_handler;





	inline void refresh() const { SDL_GL_SwapWindow(window.sdl_window); }



	Application(ConsoleInterpreter& interpreter) : window(interpreter)
	{

	}

	// Updates timing information for the current frame
	inline void update_dt()
	{
		currentFrameTime = std::chrono::steady_clock::now(); // Get current time
		std::chrono::duration<float> elapsed = currentFrameTime - lastFrameTime; // Time elapsed
		deltaTime = elapsed.count(); // Convert to seconds
		elapsedTime += deltaTime;
		lastFrameTime = currentFrameTime; // Update the last frame time
	}

	inline void exit() {
		status = 0;
	}


	private:
		static inline float deltaTime = 0.0f;
		static inline float elapsedTime = 0.0f;
};



