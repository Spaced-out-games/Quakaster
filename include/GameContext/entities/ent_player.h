#pragma once
#include <include/GameContext/base/Entity.h>
#include <include/GameContext/components/camera.h>
#include <include/GameContext/base/Scene.h>
#include <include/GameContext/server/interpreter/ConsoleInterpreter.h>
#include <include/GameContext/utils/vector_visualizer.h>


namespace Quakaster::entities {
	struct ent_player: Entity {
		ent_player(Scene& scene, ConsoleInterpreter& interpreter, EventHandler& event_handler): Entity(scene) {
			// Add the components
			add_component<Camera>();
			//add_component<ent_controller>(event_handler, get_component<Camera>());	
			add_component<AABB>(get_component<Camera>().position);
			add_component<Scalar>(0.02,0.02,0.02);
			// set up the camera
			get_component<Camera>().set_target(*this);
			get_component<Camera>().bind_convars(interpreter);
			add_component<MoveState>();
			add_component<test_controller>(event_handler, *this);
			add_component<vector_visualizer>(get_component<MoveState>().velocity, get_component<Camera>());
		}
	};
}

using ent_player = Quakaster::entities::ent_player;