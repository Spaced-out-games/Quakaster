#pragma once
#include <include/base/Entity.h>
#include <include/components/camera.h>
#include <include/base/Scene.h>
#include <include/server/interpreter/ConsoleInterpreter.h>
#include <include/utils/vector_visualizer.h>
#include <include/utils/raycast_line.h>



struct ent_player: QKActor {

	struct tag : public Tag {};
	// Creates a player entity
	ent_player(QKScene& scene): QKActor(scene) {

		// Tag telling us this is a player
		add<ent_player::tag>();

		// Holds the orientation of the player
		add<Transform>();
		add<Camera>(get<Transform>());
		add<AABB>(1.0,3.0,1.0);

		add<Scalar>(0.02,0.02,0.02);
		// set up the camera
		get<Camera>().set_target(*this);
		add<MoveState>();
		add<QMovement>(*this);
		//add<vector_visualizer>();
	}

	// gets the Transform
	inline Transform& transform() { return get<Transform>(); }
		
	// Get the Camera
	inline Camera& camera() { return get<Camera>(); }
		
	// gets the axis-aligned bounding box
	inline AABB& aabb() { return get<AABB>(); }

	// gets the scalar for use by the vector_visualizer
	inline Scalar& scalar() { return get<Scalar>(); }

	// gets the move state for use by the physics simulation and input


};
