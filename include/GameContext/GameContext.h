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


#include <include/GameContext/UI/Spedometer.h>

#include <include/GameContext/entities/ent_cube.h>
#include <include/GameContext/resources/res_texture.h>
#include <include/GameContext/resources/res_shader.h>
#include <include/GameContext/utils/vector_visualizer.h>
#include <include/GameContext/components/AABB.h>
#include <include/GameContext/entities/ent_player.h>

static float deltaTime = 0.0f;

using namespace Quakaster;


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


	static EventHandler event_handler;

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


	// draws the UI
	inline void draw_ui() {
		ImGui_ImplSDL2_NewFrame();
		ImGui_ImplOpenGL3_NewFrame();
		ImGui::NewFrame();
		app.ui_context.tick(scene);
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	}

	// Refreshes the frame and displays it to the screen
	inline void refresh() { SDL_GL_SwapWindow(app.window.sdl_window); }
	virtual void init()
	{
		glLineWidth(4.0f);
		glClearColor(bg_color.r, bg_color.g, bg_color.b, 1.00f);

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
		// Create the player
		Entity player = ent_player(scene, interpreter, event_handler);

		// Create the floor
		ent_cube floor(scene);


		ISystem* AABB_renderer = new components::AABB::system();
		ISystem* vector_renderer = new components::vector_visualizer::system();
		ISystem* mesh_renderer = new components::Mesh::system();
		

		auto& cam_controller = player.get_component<ent_controller>();


		

		app.ui_context.add_UIElement(new spedometer(player.get_component<ent_controller>().velocity, player.get_component<ent_controller>().speed));

		vector_renderer->init(scene);
		AABB_renderer->init(scene);



			while (running) {

				update_dt();

					/* apply friction
					cam_controller.applyMovement();
					if (!cam_controller.moving) {
						if (cam_controller.in_air) {
							//cam_controller.velocity *= 0.85;
						}
						else cam_controller.velocity *= pow(0.01f, deltaTime);

					}
					else cam_controller.wish_dir = glm::normalize(cam_controller.wish_dir) * cam_controller.speed;
					*/

					player.get_component<Camera>().move(cam_controller.velocity * deltaTime);



				glClear(GL_COLOR_BUFFER_BIT);

					mesh_renderer->tick(scene);
					vector_renderer->tick(scene);
					AABB_renderer->tick(scene);




				input_delegate.update();


				draw_ui();


				refresh();
			}
		
	}
};



EventHandler GameContext::event_handler;

/*
FYI: Current logic outline:
	call GameContext::init()
	create the cube vertex data buffers
	create player and cube entities, attaching components as you go
	call AABB::init() and vector_visualizer::init()


	while running:
		Apply velocity to player

		Clear the screen (glClear(GL_COLOR_BUFFER_BIT))

		Draw meshes, AABBs, and vector visualizers

		Set scaled_velocity (only used for spedometer)

		Take in input through input_delegate

		Draw UI


*/