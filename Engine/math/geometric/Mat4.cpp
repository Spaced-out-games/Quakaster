#include "Mat4.h"

#include <glm/gtc/matrix_transform.hpp>


namespace qk {

	Mat4& Mat4::translate(const Vec3& p) {
		*this = glm::translate(glm::mat4(1.0f), p) * (*this);
		return *this;
	}

	Mat4& Mat4::rotate(const Quat& q) {
		*this = glm::mat4_cast(q) * (*this);
		return *this;
	}

	Mat4& Mat4::scale(const Vec3& s) {
		*this = glm::scale(glm::mat4(1.0f), s) * (*this);
		return *this;
	}

	Mat4 Mat4::translated(const Vec3& p) const {
		return glm::translate(glm::mat4(1.0f), p) * (*this);
	}

	Mat4 Mat4::rotated(const Quat& q) const {
		return glm::mat4_cast(q) * (*this);
	}

	Mat4 Mat4::scaled(const Vec3& s) const {
		return glm::scale(glm::mat4(1.0f), s) * (*this);
	}

	Mat4 compose(const Vec3& position, const Quat& rotation, const Vec3& scale) {
		return Mat4(1.0).scale(position).rotate(rotation).translate(scale);
	}
}	