#pragma once
#include <include/thirdparty/entt.hpp>
#include "include/GameContext/window/window.h"
#include "include/GameContext/client/application.h"
#include <imgui.h>
#include <backends/imgui_impl_sdl2.h>
#include <backends/imgui_impl_opengl3.h>
#include <include/GameContext/IO/controller.h>
#include <include/GameContext/Scene.h>
#include <include/GameContext/ECS/Entity.h>

std::string my_command(std::span<Token> args) {
	// Handle your command's logic here
	return "Command executed successfully!";
}

struct GameContext
{
	// Whether or not the game is running
	int running = 1;

	// Interprets console commands
	ConsoleInterpreter interpreter;

	Scene scene;

	ent dummy{scene.registry};

	//
	entt::dispatcher event_handler;
	Application app;

	// absolutely NEEDS an event handler and UI context. 
	Controller controller;

	// Constructor
	GameContext() : app(event_handler, interpreter), controller(event_handler, app.ui_context, event_handler) {
		if (controller.IOHandler == nullptr) { controller.init(); }
		interpreter.add_command("quit", *my_command);
		//interpreter.add_convar("quit", console_fn(GameContext::exit));
	}
	
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
	virtual void init()
	{
		// Use the scene's make_entity function to create an entity and get a handle
		entt::handle handle = dummy.get_handle(); // Use the dummy entity's handle
		dummy.add<EventListener>(event_handler, KEY_EVENTS);

		// Emplace the EventListener directly into the registry
		//auto& listener = scene.registry.emplace<EventListener>(handle.entity(), event_handler, event_type_mask::KEY_PRESS);

		// Capture the handle in the lambda
		//listener.on_keyPress = [handle](KeyPressEvent& evt) {
			// Access the entity through the handle and print the key code
			//std::cout << "Key '" << SDL_GetKeyName(evt.code) << "' (code: " << evt.code << ") pressed for entity: "
		//		<< entt::to_integral(handle.entity()) << "\n";
		//};
	}

	// Game loop
	virtual void run()
	{
		init();
		while (running)
		{
			//std::cout << 
			//std::cout << dummy.inspect();
			controller.update();
			begin_ui();
			draw_ui();
			end_ui();
			
			refresh();
		}
	}
};

