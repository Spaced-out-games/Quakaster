#pragma once
#include <include/thirdparty/entt.hpp>
#include "include/GameContext/window/window.h"
#include "include/GameContext/client/application.h"
#include <imgui.h>
#include <backends/imgui_impl_sdl2.h>
#include <backends/imgui_impl_opengl3.h>
#include <include/GameContext/IO/controller.h>
#include <include/GameContext/Scene.h>
#include <include/GameContext/components/camera.h>
#include <include/GameContext/components/Framebuffer.h>
#include <include/GameContext/graphics/VAO.h>
#include <include/GameContext/graphics/VBO.h> // Just in case

//#include <resources/shaders/default.frag>

#include <include/GameContext/resources/res_shader.h>
#include <include/GameContext/resources/res_mesh.h>

std::string my_command(ConsoleInterpreter& interpreter, std::span<Token> args) {
	// Handle your command's logic here
	return "Command executed successfully!";
}

struct GameContext
{
	glm::vec3 bg_color = { 0.45f, 0.55f, 0.60f };
	Camera camera;
	// Whether or not the game is running
	int running = 1;

	// Interprets console commands
	ConsoleInterpreter interpreter;

	Scene scene;

	//
	entt::dispatcher event_handler;
	Application app;

	// absolutely NEEDS an event handler and UI context. 
	Controller controller;
	
	// Constructor
	GameContext() : app(event_handler, interpreter), controller(event_handler, app.ui_context, event_handler) {
		if (controller.IOHandler == nullptr) { controller.init(); }
		interpreter.add_convar("game_running", running);
		interpreter.add_command("quit", GameContext::quit);
		interpreter.add_command("exit", GameContext::quit); // alias

		interpreter.add_convar("bg_color_value", bg_color, false);
		interpreter.add_command("bg_color", bg_color_fn);

		
		
		camera.bind_convars(interpreter);
	}

	static void bg_color_fn(console_message& msg, ConsoleInterpreter& interpreter, std::span<Token> args) {

		glm::vec3* target = (glm::vec3*)interpreter.convars["bg_color_value"].target;
		if (args.size() == 1)
		{
			float bw;
			if (args[0].token_type == TYPE_FLOAT) {
				bw = std::get<float>(args[0].value);
				target->x = bw;
				target->y = bw;
				target->z = bw;
				return;
			}
			else if (args[0].token_type == TYPE_INTEGER) {
				bw = (float)std::get<int>(args[0].value) / 255.0f;
				target->x = bw;
				target->y = bw;
				target->z = bw;
				return;
			}
		}


		if (args.size() == 3)
		{
			// set X
			if (args[0].token_type == TYPE_FLOAT) { target->x = std::get<float>(args[0].value); }
			else if (args[0].token_type == TYPE_INTEGER) { target->x = (float)std::get<int>(args[0].value) / 255.0f; }

			// set Y
			if (args[1].token_type == TYPE_FLOAT) { target->y = std::get<float>(args[1].value); }
			else if (args[1].token_type == TYPE_INTEGER) { target->y = (float)std::get<int>(args[1].value) / 255.0f; }

			// set Z
			if (args[2].token_type == TYPE_FLOAT) { target->z = std::get<float>(args[2].value); }
			else if (args[2].token_type == TYPE_INTEGER) { target->z = (float)std::get<int>(args[2].value) / 255.0f; }


		}




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
		//glClear(GL_COLOR_BUFFER_BIT);
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}

	// Refreshes the frame and displays it to the screen
	inline void refresh() { SDL_GL_SwapWindow(app.window.sdl_window); }
	virtual void init() {}

	static void quit(console_message& msg, ConsoleInterpreter& interpreter, std::span<Token> args) {
		//auto shader = res_shader::load("default", "resources/shaders/default.vert", "resources/shaders/default.frag");

		

		

		//shader_handle.first->second.handle().get()->bind()


		interpreter.set_convar("game_running", 0);
		msg.message = "Quiting game...";
		return;
	}


	// Game loop
	virtual void run()
	{


		// Load a shader resource
		Shader s = res_shader::load("test", "resources/shaders/default.vert", "resources/shaders/default.frag");
		// Points to draw
		std::vector<default_vertex_t> points = {
	{{0.0f, 1.1547f, 0.0f}}, // Top vertex (approx 2 / sqrt(3))
	{{-0.5f, -0.5774f, 0.0f}}, // Bottom-left vertex (-1 / sqrt(3))
	{{0.5f, -0.5774f, 0.0f}},  // Bottom-right vertex
		};

		// Create VAO and VBO
		auto entity1 = scene.registry.create();
		entt::handle handle1{ scene.registry, entity1 };
		Mesh mesh1(handle1, points, s); // Mesh with first shader

		// Add VBO to VAO

		//default_vertex_t::set_pointers();
		//VBO::unbind();
		//vao.unbind();

		init();
		while (running)
		{
			glClearColor(bg_color.r, bg_color.g, bg_color.b, 1.00f);
			s->bind();
			//vao.bind();
			//glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(points.size()));
			Mesh::draw_all(scene.registry);

			controller.update();
			begin_ui();
			draw_ui();
			end_ui();
			

			refresh();
		}
	}
};

