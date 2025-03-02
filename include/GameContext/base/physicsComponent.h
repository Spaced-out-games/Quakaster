#pragma once
#include <include/GameContext/base/Component.h>
#include <include/GameContext/base/System.h>
#include <include/thirdparty/glm/vec3.hpp>

struct PhysicsComponent : Component {
	glm::vec3 velocity;
	glm::vec3 accel;
	// UNUSED
	float mass;

	float ground_friction = 0.001f;
	float air_friction = 0.0f;
	
};