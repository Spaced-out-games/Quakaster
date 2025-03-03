#pragma once
#include "include/GameContext/window/window.h"
#include "include/GameContext/client/application.h"
#include <chrono>
#include <imgui.h>
#include <backends/imgui_impl_sdl2.h>
#include <backends/imgui_impl_opengl3.h>
#include <include/GameContext/components/ent_controller.h>
#include <include/GameContext/base/Quakaster.h>
#include <include/GameContext/UI/Spedometer.h>

#include <include/GameContext/entities/ent_cube.h>
#include <include/GameContext/resources/res_texture.h>
#include <include/GameContext/resources/res_shader.h>
#include <include/GameContext/utils/vector_visualizer.h>
#include <include/GameContext/components/AABB.h>
#include <include/GameContext/entities/ent_player.h>
#include <include/GameContext/server/server.h>


using namespace Quakaster;


// mock server
static inline Server sv;



struct GameContext : Application
{
	// Constructor
	GameContext() : Application(sv.interpreter)//, input_delegate(ui_context, event_handler)
	{
		//if (input_delegate.IOHandler == nullptr) { input_delegate.init(); }
		//sv.interpreter.add_convar("game_running", status);
		//sv.interpreter.add_command("quit", GameContext::quit);
		//sv.interpreter.add_command("exit", GameContext::quit); // alias

		//sv.interpreter.add_convar("bg_color_value", bg_color, false);
		//sv.interpreter.add_command("bg_color", bg_color_fn);






	}
	static void console_log(std::string message, console_color color = console_color::DEFAULT_TEXT)
	{
		event_handler.trigger(console_message{ message, color });

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

	static void quit(console_message& msg, ConsoleInterpreter& interpreter, std::span<Token> args) {
		interpreter.set_convar("game_running", 0);
		msg.message = "Quiting game...";
		return;
	}
	// draws the UI
	/*
	inline void draw_ui() {
		ImGui_ImplSDL2_NewFrame();
		ImGui_ImplOpenGL3_NewFrame();
		ImGui::NewFrame();
		ui_context.tick(cl.scene);
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	}*/

	// Refreshes the frame and displays it to the screen
	inline void refresh() { SDL_GL_SwapWindow(window.sdl_window); }




	int run() override {
		// Initialize the app state

		systems.push_back(new InputDelegate(event_handler));

		systems.push_back(new components::Mesh::system());
		systems.push_back(new components::AABB::system());
		systems.push_back(new components::vector_visualizer::system());
		systems.push_back(new UIContext(event_handler, sv.interpreter, window.get_renderer(), window, *dynamic_cast<InputDelegate*>(systems[0])));

		//systems.push_back(&input_delegate);

		glLineWidth(4.0f);
		glClearColor(bg_color.r, bg_color.g, bg_color.b, 1.00f);
		for (auto* system : systems)
		{
			system->init(cl.scene);
		}
		entities.emplace_back(new ent_player(cl.scene, sv.interpreter, event_handler));
		entities.emplace_back(new ent_cube(cl.scene));




		// run everything
		while (status == 1)
		{
			update_dt();
			glClear(GL_COLOR_BUFFER_BIT);
			for (auto* system : systems)
			{
				system->tick(cl.scene);
			}
			entities[0]->get_component<Camera>().owner_transform.position += entities[0]->get_component<MoveState>().velocity * Application::get_deltaTime();
			refresh();
		}
		return status;
	}
}