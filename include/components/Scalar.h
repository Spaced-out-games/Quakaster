#pragma once
#include <include/base/Component.h>
#include <include/base/System.h>
#include <include/thirdparty/glm/vec3.hpp>

struct Scalar : Component {
	Scalar() = default;
	Scalar(float x, float y, float z) : scale({ x, y, z }) {}
	glm::vec3 scale = { 1.0f,1.0f,1.0f };
};

// Good for utility components like vector_visualizer and AABB's rendering
struct control_point : glm::vec3, Component {};