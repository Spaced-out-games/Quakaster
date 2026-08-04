#include "Quat.h"
#include "core.h"
#include <glm/geometric.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/epsilon.hpp>

namespace qk
{

    Quat::Quat()
        : m_Data(1.f, 0.f, 0.f, 0.f)
    {
    }

    Quat::Quat(float x, float y, float z, float w)
        : m_Data(w, x, y, z)
    {
    }

    Quat::Quat(const glm::quat& q)
        : m_Data(q)
    {
    }

    Quat::operator const glm::quat& () const
    {
        return m_Data;
    }

    float& Quat::x() { return m_Data.x; }
    float& Quat::y() { return m_Data.y; }
    float& Quat::z() { return m_Data.z; }
    float& Quat::w() { return m_Data.w; }

    const float& Quat::x() const { return m_Data.x; }
    const float& Quat::y() const { return m_Data.y; }
    const float& Quat::z() const { return m_Data.z; }
    const float& Quat::w() const { return m_Data.w; }

    float Quat::Length() const
    {
        return glm::length(m_Data);
    }

    float Quat::LengthSquared() const
    {
        return glm::dot(m_Data, m_Data);
    }

    void Quat::Normalize()
    {
        m_Data = glm::normalize(m_Data);
    }

    Quat Quat::Normalized() const
    {
        return Quat(glm::normalize(m_Data));
    }

    void Quat::Invert()
    {
        m_Data = glm::inverse(m_Data);
    }

    Quat Quat::Inverse() const
    {
        return Quat(glm::inverse(m_Data));
    }

    bool Quat::operator==(const Quat& rhs) const
    {
        return glm::all(glm::epsilonEqual(m_Data, rhs.m_Data, QK_EPSILON));
    }

    bool Quat::operator!=(const Quat& rhs) const
    {
        return !(*this == rhs);
    }

    Quat Quat::Identity()
    {
        return Quat();
    }

    Quat operator+(const Quat& a, const Quat& b)
    {
        return Quat(static_cast<const glm::quat&>(a) + static_cast<const glm::quat&>(b));
    }

    Quat operator-(const Quat& a, const Quat& b)
    {
        return Quat(static_cast<const glm::quat&>(a) - static_cast<const glm::quat&>(b));
    }

    Quat operator*(const Quat& q, float s)
    {
        return Quat(static_cast<const glm::quat&>(q) * s);
    }

    Quat operator*(float s, const Quat& q)
    {
        return Quat(s * static_cast<const glm::quat&>(q));
    }

    Quat operator*(const Quat& lhs, const Quat& rhs)
    {
        return Quat(
            static_cast<const glm::quat&>(lhs) *
            static_cast<const glm::quat&>(rhs));
    }

    Quat operator/(const Quat& q, float s)
    {
        return Quat(static_cast<const glm::quat&>(q) / s);
    }

    Quat Multiply(const Quat& a, const Quat& b)
    {
        return Quat(
            static_cast<const glm::quat&>(a) *
            static_cast<const glm::quat&>(b));
    }

    float Dot(const Quat& a, const Quat& b)
    {
        return glm::dot(
            static_cast<const glm::quat&>(a),
            static_cast<const glm::quat&>(b));
    }

    Quat Slerp(const Quat& a, const Quat& b, float t)
    {
        return Quat(glm::slerp(
            static_cast<const glm::quat&>(a),
            static_cast<const glm::quat&>(b),
            t));
    }

    Quat Nlerp(const Quat& a, const Quat& b, float t)
    {
        return Quat(glm::normalize(glm::mix(
            static_cast<const glm::quat&>(a),
            static_cast<const glm::quat&>(b),
            t)));
    }

    Vec3 Rotate(const Quat& q, const Vec3& v)
    {
        return Vec3(glm::quat(q) *glm::vec3(v));
    }

    Vec4 Rotate(const Quat& q, const Vec4& v)
    {
        glm::vec3 xyz =
            static_cast<const glm::quat&>(q) *
            glm::vec3(
                static_cast<const glm::vec4&>(v));

        return Vec4(xyz.x, xyz.y, xyz.z, v.w());
    }

    Quat AngleAxis(float radians, const Vec3& axis)
    {
        return Quat(glm::angleAxis(radians, glm::vec3(axis)));
    }

    Quat Euler(float pitch, float yaw, float roll)
    {
        return Quat(glm::quat(glm::vec3(
            pitch,
            yaw,
            roll)));
    }

    Vec3 EulerAngles(const Quat& q)
    {
        return Vec3(glm::eulerAngles(
            static_cast<const glm::quat&>(q)));
    }

}