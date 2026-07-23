#include <glm/vec3.hpp>
#include <glm/geometric.hpp>


namespace qk {

	using Vec3 = glm::vec3;
	
	inline constexpr float length(const Vec3& target) {
		return glm::length(target);
	}

	inline constexpr float dist(const Vec3& a, const Vec3& b) {
		return glm::distance(a, b);
	}

	inline constexpr float dot(const Vec3& a, const Vec3& b) {
		return glm::dot(a, b);
	}

	inline constexpr Vec3 cross(const Vec3& a, const Vec3& b) {
		return glm::cross(a, b);
	}


	inline Vec3 normalized(const Vec3& target) {
		return glm::normalize(target);
	}

	//inline Vec3 flip_forward(const Vec3& )


}