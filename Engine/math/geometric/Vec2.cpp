#include "Vec2.h"

#include <glm/geometric.hpp>
#include <glm/gtc/epsilon.hpp>
#include <core.h>
namespace qk
{

    [[nodiscard]] float length(const Vec2& target) {
        return glm::length((const glm::vec2&)target);
    }


    [[nodiscard]] float length_squared(const Vec2& target) {
        return glm::dot((const glm::vec2&)target, (const glm::vec2&)target);
    }
    void normalize(Vec2& target) {
        target = glm::normalize((const glm::vec2&)target);
    }
    [[nodiscard]] Vec2 normalized(const Vec2& target) {
        return glm::normalize((const glm::vec2&)target);
    }
    [[nodiscard]] float dot(const Vec2& a, const Vec2& b) {
        return glm::dot((const glm::vec2&)a, (const glm::vec2&)b);
    }
    [[nodiscard]] float cross(const Vec2& a, const Vec2& b) {
        return a.x * b.y - a.y * b.x;
    }

    
    

    


}

std::ostream& operator<<(std::ostream& os, const qk::Vec2& pos) {
    return os << "Vec2(" << pos.x << ", " << pos.y << ")";
}


std::ostream& operator<<(std::ostream& os, const qk::Vec2i& pos) {
    return os << "Vec2i(" << pos.x << ", " << pos.y << ")";
}