#pragma once

#include <glm/gtc/quaternion.hpp>

#include "Vec3.h"
#include "Vec4.h"
#include "geometric.h"
#include <ostream>
namespace qk
{
    class Quat
    {
    public:
        Quat();
        Quat(float x, float y, float z, float w);
        explicit Quat(const glm::quat& q);

        explicit operator const glm::quat& () const;

        float& x();
        float& y();
        float& z();
        float& w();

        const float& x() const;
        const float& y() const;
        const float& z() const;
        const float& w() const;

        float Length() const;
        float LengthSquared() const;

        void Normalize();
        Quat Normalized() const;

        void Invert();
        Quat Inverse() const;

        bool operator==(const Quat&) const;
        bool operator!=(const Quat&) const;

        static Quat Identity();

    private:
        glm::quat m_Data;
    };



    // Arithmetic
    Quat operator+(const Quat&, const Quat&);
    Quat operator-(const Quat&, const Quat&);
    Quat operator*(const Quat&, const Quat&);
    Quat operator*(const Quat&, float);
    Quat operator*(float, const Quat&);
    Quat operator/(const Quat&, float);

    // Quaternion math
    float Dot(const Quat&, const Quat&);
    Quat Slerp(const Quat&, const Quat&, float);
    Quat Nlerp(const Quat&, const Quat&, float);

    // Quaternion/vector
    Vec3 Rotate(const Quat&, const Vec3&);
    Vec4 Rotate(const Quat&, const Vec4&);

    // Construction
    Quat AngleAxis(float radians, const Vec3& axis);

    Quat Euler(float pitch, float yaw, float roll);

    // Extraction
    Vec3 EulerAngles(const Quat&);

    std::ostream& operator<<(std::ostream& os, const qk::Quat& rot) {
        return os << "Vec4(" << rot.x() << ", " << rot.y() << ", " << rot.z() << ", " << rot.w() << ")";
    }
}