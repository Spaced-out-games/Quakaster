#pragma once
#include <include/thirdparty/entt.hpp>
#include "include/GameContext/window/window.h"
#include "include/GameContext/client/application.h"
#include <imgui.h>
#include <backends/imgui_impl_sdl2.h>
#include <backends/imgui_impl_opengl3.h>



struct GameContext
{
	// Whether or not the game is running
	bool running = true;

	// Interprets console commands
	ConsoleInterpreter interpreter;

	//
	eventHandler event_handler;
	Application app;


	// Constructor
	GameContext() : app(event_handler, interpreter) {}

	// Prepares for the UI draw call
	inline void begin_ui()
	{
		ImGui_ImplSDL2_NewFrame();
		ImGui_ImplOpenGL3_NewFrame();
		ImGui::NewFrame();
	}

	// draws the UI
	inline void draw_ui() { app.ui_context.draw(); }

	// ends the UI draw call
	inline void end_ui()
	{
		ImGui::Render();
		glClear(GL_COLOR_BUFFER_BIT);
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}

	// Refreshes the frame and displays it to the screen
	inline void refresh()
	{
		SDL_GL_SwapWindow(app.window.sdl_window);
	}

	// Game loop
	virtual void run()
	{
		while (running)
		{

			begin_ui();
			draw_ui();
			end_ui();
			
			refresh();
		}
	}
};