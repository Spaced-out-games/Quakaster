#pragma once
#include <glm/mat4x4.hpp>
#include "Vec3.h"
#include "Quat.h"
#include "Mat4.h"



namespace qk {
	//todo: actually finish lmao
	struct Mat4 : glm::mat4 {
		using glm::mat4::mat4;

		constexpr Mat4(const glm::mat4& v): glm::mat4(v) {}

	};

	Mat4 compose(const Vec3& position, const Quat& rotation, const Vec3& scale);


	using CMat4 = Mat4;

}