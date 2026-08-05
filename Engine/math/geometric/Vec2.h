#pragma once

#include <glm/vec2.hpp>
#include "geometric.h"
#include <ostream>

namespace qk
{

    struct Vec2 : glm::vec2
    {
        using glm::vec2::vec2;

        [[nodiscard]] static constexpr Vec2 zero() { return { 0, 0 }; }
        [[nodiscard]] static constexpr Vec2 one() { return { 1, 1 }; }
        [[nodiscard]] static constexpr Vec2 up() { return { 0, 1 }; }
        [[nodiscard]] static constexpr Vec2 right() { return { 1, 0 }; }

        // just forward to GLM
        constexpr Vec2(const glm::vec2& v): glm::vec2(v) {}
    };

    // defaults
    [[nodiscard]] float length(const Vec2& target);
    [[nodiscard]] float length_squared(const Vec2& target);
    void normalize(Vec2& target);
    [[nodiscard]] Vec2 normalized(const Vec2& target);
    [[nodiscard]] float dot(const Vec2& a, const Vec2& b);
    [[nodiscard]] float cross(const Vec2& a, const Vec2& b);

    [[nodiscard]] inline bool operator<=(const Vec2& lhs, const Vec2& rhs)
    {
        return lhs.x <= rhs.x &&
            lhs.y <= rhs.y;
    }

    [[nodiscard]] inline bool operator>=(const Vec2& lhs, const Vec2& rhs)
    {
        return lhs.x >= rhs.x &&
            lhs.y >= rhs.y;
    }


    struct Vec2i : glm::ivec2
    {
        using glm::ivec2::ivec2;

        [[nodiscard]] static constexpr Vec2i zero() { return { 0, 0 }; }
        [[nodiscard]] static constexpr Vec2i one() { return { 1, 1 }; }
        [[nodiscard]] static constexpr Vec2i up() { return { 0, 1 }; }
        [[nodiscard]] static constexpr Vec2i right() { return { 1, 0 }; }

        // just forward to GLM
        constexpr Vec2i(const glm::ivec2& v) : glm::ivec2(v) {}

    };



}

std::ostream& operator<<(std::ostream& os, const qk::Vec2& pos);

std::ostream& operator<<(std::ostream& os, const qk::Vec2i& pos);