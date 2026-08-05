#pragma once

#include <glm/vec4.hpp>
#include "geometric.h"
#include <ostream>


namespace qk
{

    struct Vec4 : glm::vec4
    {
        using glm::vec4::vec4;

        [[nodiscard]] static constexpr Vec4 zero() { return { 0, 0, 0, 0 }; }
        [[nodiscard]] static constexpr Vec4 one() { return { 1, 1, 1, 0 }; }
        [[nodiscard]] static constexpr Vec4 Up() { return { 0, 1, 0, 0 }; }
        [[nodiscard]] static constexpr Vec4 Right() { return { 1, 0, 0, 0 }; }
        [[nodiscard]] static constexpr Vec4 Forward() { return { 0, 0,-1, 0 }; }

        // just forward to GLM
        constexpr Vec4(const glm::vec4& v) : glm::vec4(v) {}
    };

    // defaults
    [[nodiscard]] float length(const Vec4& target);
    [[nodiscard]] float length_squared(const Vec4& target);
    void normalize(Vec4& target);
    [[nodiscard]] Vec4 normalized(const Vec4& target);
    [[nodiscard]] float dot(const Vec4& a, const Vec4& b);
    [[nodiscard]] float cross(const Vec4& a, const Vec4& b);


    struct Vec4i : glm::ivec4
    {
        using glm::ivec4::ivec4;

        [[nodiscard]] static constexpr Vec4i zero() { return { 0, 0, 0, 0 }; }
        [[nodiscard]] static constexpr Vec4i one() { return { 1, 1, 1, 0 }; }
        [[nodiscard]] static constexpr Vec4i Up() { return { 0, 1, 0, 0 }; }
        [[nodiscard]] static constexpr Vec4i Right() { return { 1, 0, 0, 0 }; }
        [[nodiscard]] static constexpr Vec4i Forward() { return { 0, 0,-1, 0 }; }

        // just forward to GLM
        constexpr Vec4i(const glm::ivec4& v) : glm::ivec4(v) {}

    };



}

std::ostream& operator<<(std::ostream& os, const qk::Vec4& pos);

std::ostream& operator<<(std::ostream& os, const qk::Vec4i& pos);