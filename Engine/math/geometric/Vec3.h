#pragma once

#include <glm/vec3.hpp>
#include "geometric.h"
#include <ostream>

namespace qk
{

    struct Vec3 : glm::vec3
    {
        using glm::vec3::vec3;

        [[nodiscard]] static constexpr Vec3 zero() { return { 0, 0, 0 }; }
        [[nodiscard]] static constexpr Vec3 one() { return { 1, 1, 1 }; }
        [[nodiscard]] static constexpr Vec3 Up() { return { 0, 1, 0 }; }
        [[nodiscard]] static constexpr Vec3 Right() { return { 1, 0, 0 }; }
        [[nodiscard]] static constexpr Vec3 Forward() { return { 0, 0,-1 }; }

        // just forward to GLM
        constexpr Vec3(const glm::vec3& v) : glm::vec3(v) {}
    };

    [[nodiscard]] inline bool operator<=(const Vec3& lhs, const Vec3& rhs)
    {
        return lhs.x <= rhs.x &&
            lhs.y <= rhs.y &&
            lhs.z <= rhs.z;
    }

    [[nodiscard]] inline bool operator>=(const Vec3& lhs, const Vec3& rhs)
    {
        return lhs.x >= rhs.x &&
            lhs.y >= rhs.y &&
            lhs.z >= rhs.z;
    }


    // defaults
    [[nodiscard]] float length(const Vec3& target);
    [[nodiscard]] float length_squared(const Vec3& target);
    void normalize(Vec3& target);
    [[nodiscard]] Vec3 normalized(const Vec3& target);
    [[nodiscard]] float dot(const Vec3& a, const Vec3& b);
    [[nodiscard]] float cross(const Vec3& a, const Vec3& b);


    struct Vec3i : glm::ivec3
    {
        using glm::ivec3::ivec3;

        [[nodiscard]] static constexpr Vec3i zero() { return { 0, 0, 0 }; }
        [[nodiscard]] static constexpr Vec3i one() { return { 1, 1, 1 }; }
        [[nodiscard]] static constexpr Vec3i Up() { return { 0, 1, 0 }; }
        [[nodiscard]] static constexpr Vec3i Right() { return { 1, 0, 0 }; }
        [[nodiscard]] static constexpr Vec3i Forward() { return { 0, 0,-1 }; }

        // just forward to GLM
        constexpr Vec3i(const glm::ivec3& v) : glm::ivec3(v) {}

    };



}


std::ostream& operator<<(std::ostream& os, const qk::Vec3& pos);

std::ostream& operator<<(std::ostream& os, const qk::Vec3i& pos);