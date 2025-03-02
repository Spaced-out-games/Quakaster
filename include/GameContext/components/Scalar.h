#pragma once
#include <include/GameContext/base/Component.h>
#include <include/GameContext/base/System.h>
#include <include/thirdparty/glm/vec3.hpp>

struct Scalar : Component {
	Scalar() = default;
	Scalar(float x, float y, float z) : scale({ x, y, z }) {}
	glm::vec3 scale = { 1.0f,1.0f,1.0f };
};

struct UniformScalar : Component {
	float scale = 1.0f;
};

// Good for utility components, mostly.
struct control_point : Component {
	glm::vec3 point = { 0.0f,0.0f,0.0f };
};