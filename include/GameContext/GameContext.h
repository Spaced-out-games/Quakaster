#pragma once
#include "include/GameContext/window/window.h"
#include "include/GameContext/client/application.h"
#include <chrono>
#include <imgui.h>
#include <backends/imgui_impl_sdl2.h>
#include <backends/imgui_impl_opengl3.h>
#include <include/GameContext/components/ent_controller.h>

#include <include/GameContext/base/Quakaster.h>

// TODO: Implement 'help' and 'echo'

//#include <resources/shaders/default.frag>

#include <include/GameContext/resources/res_texture.h>
#include <include/GameContext/resources/res_shader.h>
#include <include/GameContext/utils/vector_visualizer.h>
#include <include/GameContext/components/AABB.h>

static float deltaTime = 0.0f;

using namespace Quakaster::components;


struct GameContext
{
	
	
	std::vector<Quakaster::base::ISystem> systems;
	

	// Timing variables using std::chrono
	std::chrono::steady_clock::time_point lastFrameTime;
	std::chrono::steady_clock::time_point currentFrameTime;

	glm::vec3 bg_color = { 0.0f,1.0f,1.0f };//{ 0.45f, 0.55f, 0.60f };
	// Whether or not the game is running
	int running = 1;
	

	// Interprets console commands
	ConsoleInterpreter interpreter;

	// The scene
	Quakaster::base::Scene scene;


	static eventHandler event_handler;

	Application app;

	// absolutely NEEDS an event handler and UI context. 
	InputDelegate input_delegate;
	
	// Constructor
	GameContext() : app(event_handler, interpreter), input_delegate(app.ui_context, event_handler) {
		if (input_delegate.IOHandler == nullptr) { input_delegate.init(); }
		interpreter.add_convar("game_running", running);
		interpreter.add_command("quit", GameContext::quit);
		interpreter.add_command("exit", GameContext::quit); // alias

		interpreter.add_convar("bg_color_value", bg_color, false);
		interpreter.add_command("bg_color", bg_color_fn);

		

	}
	static void console_log(std::string message,console_color color = console_color::DEFAULT_TEXT)
	{
		event_handler.trigger(console_message{message, color });

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


		glClearColor(target->r, target->g, target->b, 1.00f);

	}
	static void help_fn(console_message& msg, ConsoleInterpreter& interpreter, std::span<Token> args) {

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
	virtual void init()
	{
		glLineWidth(4.0f);
	}

	static void quit(console_message& msg, ConsoleInterpreter& interpreter, std::span<Token> args) {
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
		init();

		// Load a shader resource
		//Shader shader = res_shader::load("test", "resources/shaders/default.vert", "resources/shaders/default.frag");
		// Points to draw
		std::vector<default_vertex_t> vertices = {
			{{0.0f, 0.0f, 0.0f}},  // Vertex 0: Bottom-left-back
			{{100.0f, 0.0f, 0.0f}},  // Vertex 1: Bottom-right-back
			{{100.0f, 1.0f, 0.0f}},  // Vertex 2: Top-right-back
			{{0.0f, 1.0f, 0.0f}},  // Vertex 3: Top-left-back
			{{0.0f, 0.0f, 100.0f}},  // Vertex 4: Bottom-left-front
			{{100.0f, 0.0f, 100.0f}},  // Vertex 5: Bottom-right-front
			{{100.0f, 1.0f, 100.0f}},  // Vertex 6: Top-right-front
			{{0.0f, 1.0f, 100.0f}},  // Vertex 7: Top-left-front
		};


		std::vector<uint32_t> indices = {
			// Back face
			0, 2, 1,  // Flipped 2 and 1
			0, 3, 2,  // Flipped 3 and 2
			// Front face
			5, 6, 4,  // Flipped 6 and 4
			6, 7, 4,  // Flipped 7 and 4
			// Left face
			0, 7, 3,  // Flipped 7 and 3
			0, 4, 7,  // Flipped 4 and 7
			// Right face
			1, 6, 5,  // Flipped 6 and 5
			1, 2, 6,  // Flipped 2 and 6
			// Top face
			3, 6, 2,  // Flipped 6 and 2
			3, 7, 6,  // Flipped 7 and 6
			// Bottom face
			0, 1, 5,  // Flipped 1 and 5
			0, 5, 4   // Flipped 5 and 4
		};



		auto player = entt::handle{ scene.registry, scene.registry.create() };
		player.emplace<Camera>(player);
		player.get<Camera>().bind_convars(interpreter);
		player.get<Camera>().look_at({ 0.0,0.0,0.0 });
		//player.emplace<Transform>();
		player.emplace<ent_controller>(event_handler, player.get<Camera>());


		Quakaster::base::Entity e = scene.create<Quakaster::base::Entity>();
		




		// Create an entity with a mesh
		//entt::handle{ scene.registry, scene.registry.create() };
		//vector_vis_mesh.emplace<Shader>("test", "resources/shaders/default.vert", "resources/shaders/default.frag");
		entt::handle cube = { scene.registry, scene.registry.create() };


		cube.emplace<Quakaster::components::mesh>(
			vertices,
			indices,
			"default_shader",
			"resources/shaders/default.vert",
			"resources/shaders/default.frag"
		);
		//cube.emplace<vector_visualizer>(origin);
		cube.emplace<Transform>();
		cube.emplace<Texture>("resources/images/atlas.png");

		glm::vec3 scaled_velocity;

		player.emplace<vector_visualizer>(scaled_velocity, player.get<Camera>());
		//auto& visualizer = cube.get<vector_visualizer>();
		//cube.emplace<Transform>();

		vector_visualizer::init();
		Quakaster::components::AABB::init();
		cube.emplace<AABB>(player.get<Camera>().position);

		glClearColor(bg_color.r, bg_color.g, bg_color.b, 1.00f);
		while (running)
		{

			update_dt();
			//transform.move(visualizer.vector * deltaTime);


				auto& cam_controller = player.get<ent_controller>();
				//cam_controller.velocity *= 0.999f;
//				player.get<ent_controller>().velocity
				cam_controller.applyMovement();
				if (!cam_controller.moving)
				{
					if (cam_controller.in_air)
					{
						//cam_controller.velocity *= 0.85;

					}
					else
					{
						cam_controller.velocity *= pow(0.01f, deltaTime);

					}

					//cam_controller.wish_dir = glm::normalize(wish_dir) * speed; // Normalize the direction and apply speed

				}
				else
				{
					cam_controller.wish_dir = glm::normalize(cam_controller.wish_dir) * cam_controller.speed;

				}

				player.get<Camera>().move(cam_controller.velocity * deltaTime);

			//std::cout << "dt: " << deltaTime << '\n';

			glClear(GL_COLOR_BUFFER_BIT);


			components::mesh::draw_all(scene.registry, player.get<Camera>());
			vector_visualizer::draw_all(scene.registry, player.get<Camera>());
			Quakaster::components::AABB::draw_all(scene.registry, player.get<Camera>());

			scaled_velocity = player.get<ent_controller>().velocity / 100.0f;

			input_delegate.update();
			begin_ui();
			draw_ui();
			std::string string = std::to_string((int)(glm::length(player.get<ent_controller>().velocity) * 100.0f));


			/*
			void draw_speed() {
				ImGui::SetNextWindowPos(ImVec2(100, 100), ImGuiCond_Always); // Set position
				ImGui::SetNextWindowSize(ImVec2(200, 100), ImGuiCond_Always); // Set size

				ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f); // Remove border
				ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0)); // Remove padding
				ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0, 0, 0, 0)); // Transparent background

				if (ImGui::Begin("Invisible Window", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse)) {
					ImGui::Text("Speed");
					ImGui::ProgressBar(glm::length(player.get<ent_controller>().velocity) / player.get<ent_controller>().speed, ImVec2(-1, 0), string.c_str());
				}
				ImGui::End();

				ImGui::PopStyleVar(2);
				ImGui::PopStyleColor();

			}
			*/


			end_ui();
			


			refresh();
		}
	}
};



eventHandler GameContext::event_handler;
