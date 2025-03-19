#pragma once
#include "include/window/window.h"
#include "include/client/application.h"
#include <chrono>

#include <include/components/QMovement.h>
#include <include/entities/ent_cube.h>
#include <include/components/AABB.h>
#include <include/entities/ent_player.h>
#include <include/server/server.h>
#include <include/base/logging.h>
#include <include/UI/Spedometer.h>
#include <include/bsp/bsp_plane.h>



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

		InputDelegate* dg = (InputDelegate*)cl.add_system<InputDelegate>();

		cl.add_system<MeshManager::system>();
		cl.add_system<vector_visualizer::system>();
		cl.add_system<ent_cube::system>();
		cl.add_system<AABB_system>();
		cl.add_system<RaycastMesh::system>();
		cl.add_system<QMovement::system>();
		cl.add_system<Freecam::system>();

		



		UIContext* ui = (UIContext*)cl.add_system<UIContext>(window.get_renderer(), window, *dg);



		cl.init_all();

		DevLog("-----CONSOLE_READY-----", console_color::DEFAULT_OKAY);




		glClearColor(bg_color.r, bg_color.g, bg_color.b, 1.00f);
		entities.emplace_back(new ent_player(cl.scene));
		entities.emplace_back(new ent_cube(cl.scene,glm::mat4(1.0)));

		QKEntity* player = entities[0];
		QKEntity* cube = entities[1];



		//MeshInstance floor("cube");

		MeshManager::load("D:/Quakaster/resources/levels/E1M1.obj");

		MeshInstance level("D:/Quakaster/resources/levels/E1M1.obj");

		level.submit(Matrix{}.translate(0, 10, 0));

		ui->add_UIElement(new spedometer(entities[0]->get<MoveState>().mVelocity, entities[0]->get<MoveState>().max_speed()));

		//floor.submit(
		//	glm::scale(glm::rotate(glm::mat4(1.0f), glm::radians(0.0f), { 1.0,0.0,0.0 }), { 100,1,100 })
		//);

		// run everything
		while (status == 1)
		{
			update_dt();
			std::this_thread::sleep_until(Application::get_next_frametime());
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			cl.tick_all();

			//auto a = Application::get_next_frametime();


			cube->get<MeshInstance>().submit(
				Matrix{}.translate(
					QMovement::system::ground_plane.get_point_of_intersection(
						player->get<Transform>().position,
						player->get<Transform>().get_forward_vector()
					) + glm::vec3{0,0.5,0}
				)
			);
			

			refresh();
		}
		return status;
	}
};