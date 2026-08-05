#include "Vec3.h"

#include <glm/geometric.hpp>
#include <glm/gtc/epsilon.hpp>
#include <core.h>

namespace qk {

    [[nodiscard]] float length(const Vec3& target) {
        return glm::length((const glm::vec3&)target);
    }


    [[nodiscard]] float length_squared(const Vec3& target) {
        return glm::dot((const glm::vec3&)target, (const glm::vec3&)target);
    }
    void normalize(Vec3& target) {
        target = glm::normalize((const glm::vec3&)target);
    }
    [[nodiscard]] Vec3 normalized(const Vec3& target) {
        return glm::normalize((const glm::vec3&)target);
    }
    [[nodiscard]] float dot(const Vec3& a, const Vec3& b) {
        return glm::dot((const glm::vec3&)a, (const glm::vec3&)b);
    }
    [[nodiscard]] float cross(const Vec3& a, const Vec3& b) {
        return a.x * b.y - a.y * b.x;
    }
}

std::ostream& operator<<(std::ostream& os, const qk::Vec3& pos) {
    return os << "Vec3(" << pos.x << ", " << pos.y << ", " << pos.z << ")";
}

std::ostream& operator<<(std::ostream& os, const qk::Vec3i& pos) {
    return os << "Vec3i(" << pos.x << ", " << pos.y << ", " << pos.z << ")";
}