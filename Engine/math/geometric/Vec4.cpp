#include "Vec4.h"

#include <glm/geometric.hpp>
#include <glm/gtc/epsilon.hpp>
#include "core.h"

namespace qk
{

    Vec4::Vec4() : m_Data(0.f) {}
    Vec4::Vec4(float xyzw) : m_Data(xyzw) {}
    Vec4::Vec4(float x, float y, float z, float w) : m_Data(x, y, z, w) {}
    Vec4::Vec4(const glm::vec4& v) : m_Data(v) {}

    Vec4::operator const glm::vec4& () const { return m_Data; }

    float& Vec4::x() { return m_Data.x; }
    float& Vec4::y() { return m_Data.y; }
    float& Vec4::z() { return m_Data.z; }
    float& Vec4::w() { return m_Data.w; }

    const float& Vec4::x() const { return m_Data.x; }
    const float& Vec4::y() const { return m_Data.y; }
    const float& Vec4::z() const { return m_Data.z; }
    const float& Vec4::w() const { return m_Data.w; }

    float Vec4::length() const { return glm::length(m_Data); }
    float Vec4::lengthSquared() const { return glm::dot(m_Data, m_Data); }

    void Vec4::normalize() { m_Data = glm::normalize(m_Data); }
    Vec4 Vec4::normalized() const { return Vec4(glm::normalize(m_Data)); }

    bool Vec4::operator==(const Vec4& rhs) const
    {
        return glm::all(glm::epsilonEqual(m_Data, rhs.m_Data, QK_EPSILON));
    }

    bool Vec4::operator!=(const Vec4& rhs) const
    {
        return !(*this == rhs);
    }

    Vec4 Vec4::Zero() { return { 0.f,0.f,0.f,0.f }; }
    Vec4 Vec4::One() { return { 1.f,1.f,1.f,1.f }; }

    Vec4 operator+(const Vec4& a, const Vec4& b) { return Vec4(static_cast<const glm::vec4&>(a) + static_cast<const glm::vec4&>(b)); }
    Vec4 operator-(const Vec4& a, const Vec4& b) { return Vec4(static_cast<const glm::vec4&>(a) - static_cast<const glm::vec4&>(b)); }
    Vec4 operator*(const Vec4& a, float s) { return Vec4(static_cast<const glm::vec4&>(a) * s); }
    Vec4 operator*(float s, const Vec4& a) { return Vec4(s * static_cast<const glm::vec4&>(a)); }
    Vec4 operator/(const Vec4& a, float s) { return Vec4(static_cast<const glm::vec4&>(a) / s); }

    float Dot(const Vec4& a, const Vec4& b)
    {
        return glm::dot(static_cast<const glm::vec4&>(a),
            static_cast<const glm::vec4&>(b));
    }







    Vec4i::Vec4i()
        : m_Data(0)
    {
    }

    Vec4i::Vec4i(int xyzw)
        : m_Data(xyzw)
    {
    }

    Vec4i::Vec4i(int x, int y, int z, int w)
        : m_Data(x, y, z, w)
    {
    }

    Vec4i::Vec4i(const glm::ivec4& v)
        : m_Data(v)
    {
    }

    Vec4i::operator const glm::ivec4& () const
    {
        return m_Data;
    }

    int& Vec4i::x()
    {
        return m_Data.x;
    }

    int& Vec4i::y()
    {
        return m_Data.y;
    }

    int& Vec4i::z()
    {
        return m_Data.z;
    }

    int& Vec4i::w()
    {
        return m_Data.w;
    }

    const int& Vec4i::x() const
    {
        return m_Data.x;
    }

    const int& Vec4i::y() const
    {
        return m_Data.y;
    }

    const int& Vec4i::z() const
    {
        return m_Data.z;
    }

    const int& Vec4i::w() const
    {
        return m_Data.w;
    }

    bool Vec4i::operator==(const Vec4i& rhs) const
    {
        return m_Data == rhs.m_Data;
    }

    bool Vec4i::operator!=(const Vec4i& rhs) const
    {
        return !(*this == rhs);
    }

    Vec4i Vec4i::Zero()
    {
        return { 0, 0, 0, 0 };
    }

    Vec4i Vec4i::One()
    {
        return { 1, 1, 1, 1 };
    }

    Vec4i operator+(const Vec4i& a, const Vec4i& b)
    {
        return Vec4i(
            static_cast<const glm::ivec4&>(a) +
            static_cast<const glm::ivec4&>(b));
    }

    Vec4i operator-(const Vec4i& a, const Vec4i& b)
    {
        return Vec4i(
            static_cast<const glm::ivec4&>(a) -
            static_cast<const glm::ivec4&>(b));
    }

    Vec4i operator*(const Vec4i& a, int s)
    {
        return Vec4i(
            static_cast<const glm::ivec4&>(a) * s);
    }

    Vec4i operator*(int s, const Vec4i& a)
    {
        return Vec4i(
            s * static_cast<const glm::ivec4&>(a));
    }

    Vec4i operator/(const Vec4i& a, int s)
    {
        return Vec4i(
            static_cast<const glm::ivec4&>(a) / s);
    }


}

std::ostream& operator<<(std::ostream& os, const qk::Vec4& pos) {
    return os << "Vec4(" << pos.x() << ", " << pos.y() << ", " << pos.z() << ", " << pos.w() << ")";
}

std::ostream& operator<<(std::ostream& os, const qk::Vec4i& pos) {
    return os << "Vec4i(" << pos.x() << ", " << pos.y() << ", " << pos.z() << ", " << pos.w() << ")";
}