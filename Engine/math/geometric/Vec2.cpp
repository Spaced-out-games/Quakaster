#include "Vec2.h"

#include <glm/geometric.hpp>
#include <glm/gtc/epsilon.hpp>
#include <core.h>
namespace qk
{

    Vec2::Vec2() : m_Data(0.f) {}
    Vec2::Vec2(float xy) : m_Data(xy) {}
    Vec2::Vec2(float x, float y) : m_Data(x, y) {}
    Vec2::Vec2(const glm::vec2& v) : m_Data(v) {}

    Vec2::operator const glm::vec2& () const { return m_Data; }

    float& Vec2::x() { return m_Data.x; }
    float& Vec2::y() { return m_Data.y; }

    const float& Vec2::x() const { return m_Data.x; }
    const float& Vec2::y() const { return m_Data.y; }

    float Vec2::length() const { return glm::length(m_Data); }
    float Vec2::lengthSquared() const { return glm::dot(m_Data, m_Data); }

    void Vec2::normalize() { m_Data = glm::normalize(m_Data); }
    Vec2 Vec2::normalized() const { return Vec2(glm::normalize(m_Data)); }

    bool Vec2::operator==(const Vec2& rhs) const
    {
        return glm::all(glm::epsilonEqual(m_Data, rhs.m_Data, QK_EPSILON));
    }

    bool Vec2::operator!=(const Vec2& rhs) const
    {
        return !(*this == rhs);
    }

    Vec2 Vec2::Zero() { return { 0.f,0.f }; }
    Vec2 Vec2::One() { return { 1.f,1.f }; }
    Vec2 Vec2::Up() { return { 0.f,1.f }; }
    Vec2 Vec2::Right() { return { 1.f,0.f }; }

    Vec2 operator+(const Vec2& a, const Vec2& b) { return Vec2(static_cast<const glm::vec2&>(a) + static_cast<const glm::vec2&>(b)); }
    Vec2 operator-(const Vec2& a, const Vec2& b) { return Vec2(static_cast<const glm::vec2&>(a) - static_cast<const glm::vec2&>(b)); }
    Vec2 operator*(const Vec2& a, float s) { return Vec2(static_cast<const glm::vec2&>(a) * s); }
    Vec2 operator*(float s, const Vec2& a) { return Vec2(s * static_cast<const glm::vec2&>(a)); }
    Vec2 operator/(const Vec2& a, float s) { return Vec2(static_cast<const glm::vec2&>(a) / s); }

    float Dot(const Vec2& a, const Vec2& b)
    {
        return glm::dot(static_cast<const glm::vec2&>(a),
            static_cast<const glm::vec2&>(b));
    }

    Vec2i::Vec2i() : m_Data(0) {}
    Vec2i::Vec2i(int xy) : m_Data(xy) {}
    Vec2i::Vec2i(int x, int y) : m_Data(x, y) {}
    Vec2i::Vec2i(const glm::ivec2& v) : m_Data(v) {}

    Vec2i::operator const glm::ivec2& () const { return m_Data; }

    int& Vec2i::x() { return m_Data.x; }
    int& Vec2i::y() { return m_Data.y; }

    const int& Vec2i::x() const { return m_Data.x; }
    const int& Vec2i::y() const { return m_Data.y; }

    bool Vec2i::operator==(const Vec2i& rhs) const
    {
        return m_Data == rhs.m_Data;
    }

    bool Vec2i::operator!=(const Vec2i& rhs) const
    {
        return !(*this == rhs);
    }

    Vec2i Vec2i::Zero() { return { 0,0 }; }
    Vec2i Vec2i::One() { return { 1,1 }; }
    Vec2i Vec2i::Up() { return { 0,1 }; }
    Vec2i Vec2i::Right() { return { 1,0 }; }

    Vec2i operator+(const Vec2i& a, const Vec2i& b)
    {
        return Vec2i(static_cast<const glm::ivec2&>(a) + static_cast<const glm::ivec2&>(b));
    }

    Vec2i operator-(const Vec2i& a, const Vec2i& b)
    {
        return Vec2i(static_cast<const glm::ivec2&>(a) - static_cast<const glm::ivec2&>(b));
    }

    Vec2i operator*(const Vec2i& a, int s)
    {
        return Vec2i(static_cast<const glm::ivec2&>(a) * s);
    }

    Vec2i operator*(int s, const Vec2i& a)
    {
        return Vec2i(s * static_cast<const glm::ivec2&>(a));
    }

    Vec2i operator/(const Vec2i& a, int s)
    {
        return Vec2i(static_cast<const glm::ivec2&>(a) / s);
    }


}

std::ostream& operator<<(std::ostream& os, const qk::Vec2& pos) {
    return os << "Vec2(" << pos.x() << ", " << pos.y() << ")";
}


std::ostream& operator<<(std::ostream& os, const qk::Vec2i& pos) {
    return os << "Vec2i(" << pos.x() << ", " << pos.y() << ")";
}