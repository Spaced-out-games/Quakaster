#pragma once
#include <include/thirdparty/glm/glm.hpp>

struct Velocity: public glm::vec3 {
	float speed() { return glm::length(*this); }
	glm::vec3 normalized() { return glm::normalize(*this); }
};