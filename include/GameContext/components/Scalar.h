#pragma once
#include <include/GameContext/base/Component.h>
#include <include/GameContext/base/System.h>
#include <include/thirdparty/glm/vec3.hpp>

struct Scalar : Component {
	glm::vec3 scale = { 1.0f,1.0f,1.0f };
};

struct UniformScalar : Component {
	float scale = 1.0f;
};