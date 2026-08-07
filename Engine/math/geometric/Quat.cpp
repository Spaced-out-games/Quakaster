#include "Quat.h"
#include "core.h"
#include <glm/geometric.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/epsilon.hpp>

namespace qk
{


    float Quat::length() const
    {
        return glm::length((const glm::quat&)*this);
    }

    float Quat::lengthSquared() const
    {
        return glm::dot((const glm::quat&)*this, (const glm::quat&)*this);
    }

    void Quat::normalize()
    {
        *this = glm::normalize((const glm::quat&)*this);
    }

    Quat Quat::normalized() const
    {
        return Quat(glm::normalize((const glm::quat&)*this));
    }

    void Quat::invert()
    {
        *this = glm::inverse((const glm::quat&)*this);
    }

    Quat Quat::inverse() const
    {
        return Quat(glm::inverse((const glm::quat&)*this));
    }

    



    float dot(const Quat& a, const Quat& b)
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

    Vec3 rotate(const Quat& q, const Vec3& v)
    {
        return Vec3(glm::quat(q) *glm::vec3(v));
    }

    Vec4 rotate(const Quat& q, const Vec4& v)
    {
        glm::vec3 xyz =
            static_cast<const glm::quat&>(q) *
            glm::vec3(
                static_cast<const glm::vec4&>(v));

        return Vec4(xyz.x, xyz.y, xyz.z, v.w);
    }

    Quat angleAxis(float radians, const Vec3& axis)
    {
        return Quat(glm::angleAxis(radians, glm::vec3(axis)));
    }

    Quat euler(float pitch, float yaw, float roll)
    {
        return Quat(glm::quat(glm::vec3(pitch,yaw,roll)));
    }

    Vec3 eulerAngles(const Quat& q)
    {
        return Vec3(glm::eulerAngles(
            static_cast<const glm::quat&>(q)));
    }
    float toRadians(float degrees) {
        return glm::radians(degrees);
    }

}

std::ostream& operator<<(std::ostream& os, const qk::Quat& rot) {
    return os << "Quat(" << rot.x << ", " << rot.y << ", " << rot.z << ", " << rot.w << ")";
}