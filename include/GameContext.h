#pragma once
#include "include/window/window.h"
#include "include/client/application.h"
#include <chrono>

#include <include/components/test_controller.h>
#include <include/entities/ent_cube.h>
#include <include/components/AABB.h>
#include <include/entities/ent_player.h>
#include <include/server/server.h>
#include <include/base/logging.h>

//#include <include/server/interpreter/Convar.h>


// mock server
static inline Server sv;



struct GameContext : Application
{
	// Constructor
	GameContext()
	{

		//if (input_delegate.IOHandler == nullptr) { input_delegate.init(); }
		//sv.interpreter.add_convar("game_running", status);
		//sv.interpreter.add_command("quit", GameContext::quit);
		//sv.interpreter.add_command("exit", GameContext::quit); // alias

		//sv.interpreter.add_convar("bg_color_value", bg_color, false);
		//sv.interpreter.add_command("bg_color", bg_color_fn);






	}





	// Refreshes the frame and displays it to the screen
	inline void refresh() { SDL_GL_SwapWindow(window.sdl_window); }




	int run() override {
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


		
		/*
		auto a = MeshManager::generate_mesh (
					"cube",
					vertices,
					indices,
					"default_shader",
					"resources/shaders/default.vert",
					"resources/shaders/default.frag"
		);*/

		// Initialize the app state
		cl.add_system<InputDelegate>();
		//cl.add_system<Mesh::system>();
		//cl.add_system<AABB::system>();
		//cl.add_system<vector_visualizer::system>();
		cl.add_system<UIContext>(window.get_renderer(), window, *dynamic_cast<InputDelegate*>(cl.get_systems()[0]));

		DevLog("-----CONSOLE_READY-----", console_color::DEFAULT_OKAY);


		//cl.systems.push_back(new components::Mesh::system());
		//cl.systems.push_back(new components::AABB::system());
		//cl.systems.push_back(new components::vector_visualizer::system());
		//cl.systems.push_back(new UIContext(event_handler, sv.interpreter, window.get_renderer(), window, *dynamic_cast<InputDelegate*>(cl.systems[0])));

		//cl.systems.push_back(&input_delegate);

		glLineWidth(4.0f);
		glClearColor(bg_color.r, bg_color.g, bg_color.b, 1.00f);
		cl.init_all();
		entities.emplace_back(new ent_player(cl.scene));
		entities.emplace_back(new ent_cube(cl.scene));




		// run everything
		while (status == 1)
		{
			update_dt();
			glClear(GL_COLOR_BUFFER_BIT);
			cl.tick_all();
			entities[0]->get_component<Camera>().owner_transform.position += entities[0]->get_component<MoveState>().velocity() * Application::get_deltaTime();
			refresh();
		}
		return status;
	}
};