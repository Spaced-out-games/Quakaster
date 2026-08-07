#pragma once
#include <glm/mat4x4.hpp>
#include "Vec3.h"
#include "Quat.h"



namespace qk {
	//todo: actually finish lmao
	struct Mat4 : glm::mat4 {
		using glm::mat4::mat4;

		constexpr Mat4(const glm::mat4& v): glm::mat4(v) {}

		Mat4& translate(const Vec3& position);
		Mat4& rotate(const Quat& rotation);
		Mat4& scale(const Vec3& scale);

		Mat4 translated(const Vec3& position) const;
		Mat4 rotated(const Quat& rotation) const;
		Mat4 scaled(const Vec3& scale) const;

	};

	Mat4 compose(const Vec3& position, const Quat& rotation, const Vec3& scale);


	using CMat4 = Mat4;

}