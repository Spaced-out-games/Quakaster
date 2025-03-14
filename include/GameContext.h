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
#include <include/UI/Spedometer.h>

//#include <include/server/interpreter/Convar.h>


// mock server
static inline Server sv;



struct GameContext : Application
{
	// Constructor
	GameContext()
	{








	}





	// Refreshes the frame and displays it to the screen
	inline void refresh() { SDL_GL_SwapWindow(window.sdl_window); }




	int run() override {
		

		
		//std::cout << MeshManager::info_map[a.vao].last_instance->transform_index;
		// Initialize the app state
		cl.add_system<vector_visualizer::system>();
		cl.add_system<InputDelegate>();
		cl.add_system<MeshManager::system>();
		cl.add_system<ent_cube::system>();
		cl.add_system<AABB_system>();
		cl.add_system<MoveState_system>();

		UIContext* ui = (UIContext*)cl.add_system<UIContext>(window.get_renderer(), window, *dynamic_cast<InputDelegate*>(cl.get_systems()[1]));



		cl.init_all();

		DevLog("-----CONSOLE_READY-----", console_color::DEFAULT_OKAY);



		glClearColor(bg_color.r, bg_color.g, bg_color.b, 1.00f);
		entities.emplace_back(new ent_player(cl.scene));
		entities.emplace_back(new ent_cube(cl.scene, { 0, 0, 0 }, { 100, 1, 100 }));

		ui->add_UIElement(new spedometer(entities[0]->get<MoveState>().mVelocity, entities[0]->get<MoveState>().max_speed()));

		// run everything
		while (status == 1)
		{
			update_dt();
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			cl.tick_all();
			refresh();
		}
		return status;
	}
};