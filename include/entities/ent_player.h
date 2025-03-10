#pragma once
#include <include/base/Entity.h>
#include <include/components/camera.h>
#include <include/base/Scene.h>
#include <include/server/interpreter/ConsoleInterpreter.h>
#include <include/utils/vector_visualizer.h>


namespace Quakaster::entities {
	struct ent_player: Entity {

		struct tag : public Tag {};
		// Creates a player entity
		ent_player(Scene& scene): Entity(scene) {

			// Tag telling us this is a player
			add_component<ent_player::tag>();

			// Holds the orientation of the player
			add_component<Transform>();
			add_component<Camera>(get_component<Transform>());
			add_component<AABB>(get_component<Transform>().position);
			add_component<Scalar>(0.02,0.02,0.02);
			// set up the camera
			get_component<Camera>().set_target(*this);
			add_component<MoveState>();
			add_component<test_controller>(*this);
			add_component<vector_visualizer>(get_component<MoveState>().velocity(), get_component<Camera>().owner_transform);
		}

		// gets the Transform
		inline Transform& transform() { return get_component<Transform>(); }
		
		// Get the Camera
		inline Camera& camera() { return get_component<Camera>(); }
		
		// gets the axis-aligned bounding box
		inline AABB& aabb() { return get_component<AABB>(); }

		// gets the scalar for use by the vector_visualizer
		inline Scalar& scalar() { return get_component<Scalar>(); }

		// gets the move state for use by the physics simulation and input


	};
}

using ent_player = Quakaster::entities::ent_player;