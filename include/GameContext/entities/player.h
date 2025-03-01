#pragma once
#include <include/GameContext/base/Entity.h>
#include <include/GameContext/components/camera.h>
#include <include/GameContext/base/Scene.h>
#include <include/GameContext/server/interpreter/ConsoleInterpreter.h>
#include <include/GameContext/utils/vector_visualizer.h>


namespace Quakaster::entities {
	struct ent_player: Entity {
		ent_player(Scene& scene, ConsoleInterpreter& interpreter, EventHandler& event_handler): Entity(scene) {
			add_component<Camera>();
			get_component<Camera>().bind_convars(interpreter);
			add_component<ent_controller>(event_handler, get_component<Camera>());
			add_component<vector_visualizer>(get_component<ent_controller>().velocity, get_component<Camera>());
			//get_component<Camera>().set_target()
		}
	};
}

using ent_player = Quakaster::entities::ent_player;