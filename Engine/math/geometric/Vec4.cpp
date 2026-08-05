#include "Vec4.h"

#include <glm/geometric.hpp>
#include <glm/gtc/epsilon.hpp>
#include "core.h"

namespace qk {

    [[nodiscard]] float length(const Vec4& target) {
        return glm::length((const glm::vec4&)target);
    }


    [[nodiscard]] float length_squared(const Vec4& target) {
        return glm::dot((const glm::vec4&)target, (const glm::vec4&)target);
    }
    void normalize(Vec4& target) {
        target = glm::normalize((const glm::vec4&)target);
    }
    [[nodiscard]] Vec4 normalized(const Vec4& target) {
        return glm::normalize((const glm::vec4&)target);
    }
    [[nodiscard]] float dot(const Vec4& a, const Vec4& b) {
        return glm::dot((const glm::vec4&)a, (const glm::vec4&)b);
    }
    [[nodiscard]] float cross(const Vec4& a, const Vec4& b) {
        return a.x * b.y - a.y * b.x;
    }
}

std::ostream& operator<<(std::ostream& os, const qk::Vec4& pos) {
    return os << "Vec4(" << pos.x << ", " << pos.y << ", " << pos.z << ", " << pos.w << ")";
}

std::ostream& operator<<(std::ostream& os, const qk::Vec4i& pos) {
    return os << "Vec4i(" << pos.x << ", " << pos.y << ", " << pos.z << ", " << pos.w << ")";
}