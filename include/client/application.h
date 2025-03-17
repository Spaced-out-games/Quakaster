#pragma once
#include <include/window/window.h>
#include <include/UI/UIContext.h>
#include <include/IO/InputDelegate.h>
#include <include/base/Event.h>
#include <include/client/Client.h>
#include <include/base/System.h>

Convar cl_framerate("cl_framerate", 120.0f); // just a float so that 1/cl_framerate is cheaper



struct Application
{
public:
	Application() {
		lastFrameTime = std::chrono::steady_clock::now();
	}

	std::chrono::steady_clock::time_point get_next_frametime() {
		return lastFrameTime + std::chrono::duration_cast<std::chrono::steady_clock::duration>(
			std::chrono::duration<double>(1.0 / cl_framerate.as<float>())
			);
	}

	/// <summary>
	/// Runs the application
	/// </summary>
	/// <returns>the status code upon exit</returns>
	virtual int run() = 0;

	/// <summary>
	/// Gets the time since the last frame
	/// </summary>
	/// <returns></returns>
	static const double& get_deltaTime() { return deltaTime; }
	static const double& get_elapsedTime() { return elapsedTime; }



	
	protected:
		Client cl;

		// IP address, player ID, sockets, client-side copy of the scene. also, client-side systems
		

		// color of the background
		glm::vec3 bg_color = { 0.0f,1.0f,1.0f };


		// Timing variables
		std::chrono::steady_clock::time_point lastFrameTime;
		std::chrono::steady_clock::time_point currentFrameTime;


		// Error code or exit
		int status = 1;

		// list of entities, might want to remove. Mostly for keeping things in 
		std::vector<QKEntity*> entities;


		// Content window
		Window window;



		// Refreshes the screen
		inline void refresh() const { SDL_GL_SwapWindow(window.sdl_window); }

		

		// Updates timing information for the current frame
		inline void update_dt()
		{
			currentFrameTime = std::chrono::steady_clock::now(); // Get current time
			std::chrono::duration<double> elapsed = currentFrameTime - lastFrameTime; // Time elapsed
			deltaTime = elapsed.count(); // Convert to seconds
			elapsedTime += deltaTime;
			lastFrameTime = currentFrameTime; // Update the last frame time
		}

		inline void exit() {
			status = 0;
		}


	private:
		static inline double deltaTime = 0.0f;
		static inline double elapsedTime = 0.0f;
};



