#pragma once
#include <include/thirdparty/entt.hpp>
#include "include/GameContext/window/window.h"
#include "include/GameContext/client/application.h"
#include <imgui.h>
#include <backends/imgui_impl_sdl2.h>
#include <backends/imgui_impl_opengl3.h>
#include <include/GameContext/components/ent_controller.h>
#include <include/GameContext/Scene.h>
#include <include/GameContext/components/camera.h>
#include <include/GameContext/components/Framebuffer.h>
#include <include/GameContext/graphics/VAO.h>
#include <include/GameContext/graphics/VBO.h> // Just in case
#include <chrono>

//#include <resources/shaders/default.frag>


#include <include/GameContext/resources/res_texture.h>
#include <include/GameContext/resources/res_shader.h>
#include <include/GameContext/resources/res_mesh.h>

std::string my_command(ConsoleInterpreter& interpreter, std::span<Token> args) {
	// Handle your command's logic here
	return "Command executed successfully!";
}

static float deltaTime;

struct GameContext
{
	// Timing variables
	
	// Timing variables using std::chrono
	std::chrono::steady_clock::time_point lastFrameTime;
	std::chrono::steady_clock::time_point currentFrameTime;

	glm::vec3 bg_color = { 0.45f, 0.55f, 0.60f };
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

	// Updates timing information for the current frame
	inline void update_dt()
	{
		currentFrameTime = std::chrono::steady_clock::now(); // Get current time
		std::chrono::duration<float> elapsed = currentFrameTime - lastFrameTime; // Time elapsed
		deltaTime = elapsed.count(); // Convert to seconds
		lastFrameTime = currentFrameTime; // Update the last frame time
	}
	

	// Game loop
	virtual void run()
	{


		// Load a shader resource
		//Shader shader = res_shader::load("test", "resources/shaders/default.vert", "resources/shaders/default.frag");
		// Points to draw
		std::vector<default_vertex_t> vertices = {
	{{-0.5f, -0.5f, -0.5f}}, // Vertex 0: Bottom-left-back
	{{ 0.5f, -0.5f, -0.5f}}, // Vertex 1: Bottom-right-back
	{{ 0.5f,  0.5f, -0.5f}}, // Vertex 2: Top-right-back
	{{-0.5f,  0.5f, -0.5f}}, // Vertex 3: Top-left-back
	{{-0.5f, -0.5f,  0.5f}}, // Vertex 4: Bottom-left-front
	{{ 0.5f, -0.5f,  0.5f}}, // Vertex 5: Bottom-right-front
	{{ 0.5f,  0.5f,  0.5f}}, // Vertex 6: Top-right-front
	{{-0.5f,  0.5f,  0.5f}}, // Vertex 7: Top-left-front
		};

		std::vector<uint32_t> indices = {
			// Back face
			0, 1, 2,
			0, 2, 3,
			// Front face
			4, 5, 6,
			4, 6, 7,
			// Left face
			0, 3, 7,
			0, 7, 4,
			// Right face
			1, 5, 6,
			1, 6, 2,
			// Top face
			3, 2, 6,
			3, 6, 7,
			// Bottom face
			0, 1, 5,
			0, 5, 4,
		};

		auto camera_test = scene.registry.create();
		auto handle = entt::handle{ scene.registry, camera_test };
		handle.emplace<Camera>(handle);

		handle.get<Camera>().bind_convars(interpreter);
		handle.get<Camera>().look_at({ 0.0,0.0,0.0 });
		handle.emplace<ent_controller>(event_handler, handle);

		//handle.get<Texture>().bind();


		// Create an entity with a mesh
		auto triangle_Mesh = scene.registry.create();
		entt::handle handle1{ scene.registry, triangle_Mesh };
		//handle1.emplace<Shader>("test", "resources/shaders/default.vert", "resources/shaders/default.frag");
		handle1.emplace<Texture>("resources/images/atlas.png", GL_TEXTURE_2D);
		Mesh mesh1(handle1, vertices, indices, "albedo_texture_shader", "resources/shaders/default.vert", "resources/shaders/default.frag"); // Mesh with first shader
		



		init();
		while (running)
		{
			update_dt();
			glClearColor(bg_color.r, bg_color.g, bg_color.b, 1.00f);
			glClear(GL_COLOR_BUFFER_BIT);
			scene.registry.get<Camera>(camera_test).set_shader_uniforms(handle1.get<Shader>());
			
			
			handle1.get<Texture>().bind();

			Mesh::draw_all(scene.registry);


			//t.set_uniforms(handle.get<Shader>());
			controller.update();
			begin_ui();
			draw_ui();
			end_ui();
			


			refresh();
		}
	}
};

