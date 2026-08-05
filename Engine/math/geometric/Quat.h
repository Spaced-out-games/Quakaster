#pragma once

#include <glm/gtc/quaternion.hpp>

#include "Vec3.h"
#include "Vec4.h"
#include "geometric.h"
#include <ostream>
namespace qk
{
    struct Quat: glm::quat
    {
        using glm::quat::quat;

        float length() const;
        float lengthSquared() const;

        void normalize();
        Quat normalized() const;

        void invert();
        Quat inverse() const;


        [[nodiscard]] static constexpr Vec4 identity() { return { 0, 0, 0, 0 }; }

        Quat(const glm::quat& q)
            : glm::quat(q)
        {}
    };


    



    // Quaternion math
    [[nodiscard]] float dot(const Quat&, const Quat&);
    [[nodiscard]] Quat Slerp(const Quat&, const Quat&, float);
    [[nodiscard]] Quat Nlerp(const Quat&, const Quat&, float);

    // Quaternion/vector
    [[nodiscard]] Vec3 rotate(const Quat&, const Vec3&);
    [[nodiscard]] Vec4 rotate(const Quat&, const Vec4&);

    // Construction
    [[nodiscard]] Quat angleAxis(float radians, const Vec3& axis);

    [[nodiscard]] Quat euler(float pitch, float yaw, float roll);

    // Extraction
    [[nodiscard]] Vec3 eulerAngles(const Quat&);

    
}


std::ostream& operator<<(std::ostream& os, const qk::Quat& rot);