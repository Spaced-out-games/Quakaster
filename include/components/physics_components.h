#pragma once
#include <include/thirdparty/glm/glm.hpp>

// It's literally just a vec3
struct Velocity: public glm::vec3 {
	float speed() { return glm::length(*this); }
	glm::vec3 normalized() { return glm::normalize(*this); }
};

