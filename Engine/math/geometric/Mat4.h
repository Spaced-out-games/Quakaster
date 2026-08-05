#pragma once
#include <glm/mat4x4.hpp>




namespace qk {
	//todo: actually finish lmao
	struct Mat4 : glm::mat4 {
		using glm::mat4::mat4;

		constexpr Mat4(const glm::mat4& v): glm::mat4(v) {}

	};

	using CMat4 = Mat4;

}