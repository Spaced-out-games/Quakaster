#include "Vec3.h"

#include <glm/geometric.hpp>
#include <glm/gtc/epsilon.hpp>
#include <core.h>

namespace qk
{

    Vec3::Vec3()
        : m_Data(0.0f)
    {
    }

    Vec3::Vec3(float xyz)
        : m_Data(xyz)
    {
    }

    Vec3::Vec3(float x, float y, float z)
        : m_Data(x, y, z)
    {
    }

    Vec3::Vec3(const glm::vec3& v)
        : m_Data(v)
    {
    }

    Vec3::operator glm::vec3() const
    {
        return m_Data;
    }
    Vec3::operator Vec3i() const {
        return Vec3i{ (int)m_Data.x, (int)m_Data.y, (int)m_Data.z };
    }


    float& Vec3::x()
    {
        return m_Data.x;
    }

    float& Vec3::y()
    {
        return m_Data.y;
    }

    float& Vec3::z()
    {
        return m_Data.z;
    }

    const float& Vec3::x() const
    {
        return m_Data.x;
    }

    const float& Vec3::y() const
    {
        return m_Data.y;
    }

    const float& Vec3::z() const
    {
        return m_Data.z;
    }

    float Vec3::length() const
    {
        return glm::length(m_Data);
    }

    float Vec3::lengthSquared() const
    {
        return glm::dot(m_Data, m_Data);
    }

    void Vec3::normalize()
    {
        m_Data = glm::normalize(m_Data);
    }

    Vec3 Vec3::normalized() const
    {
        return Vec3(glm::normalize(m_Data));
    }

    bool Vec3::operator==(const Vec3& rhs) const
    {
        return glm::all(glm::epsilonEqual(m_Data, rhs.m_Data, QK_EPSILON));
    }

    bool Vec3::operator!=(const Vec3& rhs) const
    {
        return !(*this == rhs);
    }

    Vec3 Vec3::Zero()
    {
        return Vec3(0.f, 0.f, 0.f);
    }

    Vec3 Vec3::One()
    {
        return Vec3(1.f, 1.f, 1.f);
    }

    Vec3 Vec3::Up()
    {
        return Vec3(0.f, 1.f, 0.f);
    }

    Vec3 Vec3::Right()
    {
        return Vec3(1.f, 0.f, 0.f);
    }

    Vec3 Vec3::Forward()
    {
        return Vec3(0.f, 0.f, -1.f);
    }

    Vec3 operator+(const Vec3& lhs, const Vec3& rhs)
    {
        return Vec3(
            static_cast<glm::vec3>(lhs) +
            static_cast<glm::vec3>(rhs));
    }

    Vec3 operator-(const Vec3& lhs, const Vec3& rhs)
    {
        return Vec3(
            static_cast<glm::vec3>(lhs) -
            static_cast<glm::vec3>(rhs));
    }

    Vec3 operator*(const Vec3& lhs, float rhs)
    {
        return Vec3(
            static_cast<glm::vec3>(lhs) * rhs);
    }

    Vec3 operator*(float lhs, const Vec3& rhs)
    {
        return Vec3(
            lhs * static_cast<glm::vec3>(rhs));
    }

    Vec3 operator/(const Vec3& lhs, float rhs)
    {
        return Vec3(
            static_cast<glm::vec3>(lhs) / rhs);
    }

    float Dot(const Vec3& a, const Vec3& b)
    {
        return glm::dot(
            static_cast<glm::vec3>(a),
            static_cast<glm::vec3>(b));
    }

    Vec3 Cross(const Vec3& a, const Vec3& b)
    {
        return Vec3(
            glm::cross(
                static_cast<glm::vec3>(a),
                static_cast<glm::vec3>(b)));
    }




    // ---------------------------------------------------------------------------------------------------------------- //



    Vec3i::Vec3i()
        : m_Data(0)
    {
    }

    Vec3i::Vec3i(int xyz)
        : m_Data(xyz)
    {
    }

    Vec3i::Vec3i(int x, int y, int z)
        : m_Data(x, y, z)
    {
    }

    Vec3i::Vec3i(const glm::ivec3& v)
        : m_Data(v)
    {
    }

    Vec3i::operator const glm::ivec3& () const
    {
        return m_Data;
    }

    Vec3i::operator Vec3() const {
        return Vec3(m_Data.x, m_Data.y, m_Data.z);
    }

    int& Vec3i::x()
    {
        return m_Data.x;
    }

    int& Vec3i::y()
    {
        return m_Data.y;
    }

    int& Vec3i::z()
    {
        return m_Data.z;
    }

    const int& Vec3i::x() const
    {
        return m_Data.x;
    }

    const int& Vec3i::y() const
    {
        return m_Data.y;
    }

    const int& Vec3i::z() const
    {
        return m_Data.z;
    }

    bool Vec3i::operator==(const Vec3i& rhs) const
    {
        return m_Data == rhs.m_Data;
    }

    bool Vec3i::operator!=(const Vec3i& rhs) const
    {
        return !(*this == rhs);
    }

    Vec3i Vec3i::Zero()
    {
        return { 0, 0, 0 };
    }

    Vec3i Vec3i::One()
    {
        return { 1, 1, 1 };
    }

    Vec3i Vec3i::Up()
    {
        return { 0, 1, 0 };
    }

    Vec3i Vec3i::Right()
    {
        return { 1, 0, 0 };
    }

    Vec3i Vec3i::Forward()
    {
        return { 0, 0, -1 };
    }

    Vec3i operator+(const Vec3i& a, const Vec3i& b)
    {
        return Vec3i(
            static_cast<const glm::ivec3&>(a) +
            static_cast<const glm::ivec3&>(b));
    }

    Vec3i operator-(const Vec3i& a, const Vec3i& b)
    {
        return Vec3i(
            static_cast<const glm::ivec3&>(a) -
            static_cast<const glm::ivec3&>(b));
    }

    Vec3i operator*(const Vec3i& a, int s)
    {
        return Vec3i(
            static_cast<const glm::ivec3&>(a) * s);
    }

    Vec3i operator*(int s, const Vec3i& a)
    {
        return Vec3i(
            s * static_cast<const glm::ivec3&>(a));
    }

    Vec3i operator/(const Vec3i& a, int s)
    {
        return Vec3i(
            static_cast<const glm::ivec3&>(a) / s);
    }



}

std::ostream& operator<<(std::ostream& os, const qk::Vec3& pos) {
    return os << "Vec3(" << pos.x() << ", " << pos.y() << ", " << pos.z() << ")";
}

std::ostream& operator<<(std::ostream& os, const qk::Vec3i& pos) {
    return os << "Vec3i(" << pos.x() << ", " << pos.y() << ", " << pos.z() << ")";
}