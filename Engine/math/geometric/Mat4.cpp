#include "Mat4.h"

#include <glm/gtc/matrix_transform.hpp>


namespace qk {
	Mat4 compose(const Vec3& position, const Quat& rotation, const Vec3& scale) {

		Mat4 S = glm::scale({ 1.0f }, scale);
		Mat4 R = glm::mat4_cast(rotation);
		Mat4 T = glm::translate({ 1.0f }, position);

		return T * R * S;
	}
}