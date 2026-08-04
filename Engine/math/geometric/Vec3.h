#pragma once

#include <glm/vec3.hpp>
#include "geometric.h"
#include <ostream>

namespace qk
{

    class Vec3i;

    class Vec3
    {
    public:
        Vec3();
        explicit Vec3(float xyz);
        Vec3(float x, float y, float z);
        explicit Vec3(const glm::vec3& v);

        explicit operator glm::vec3() const;
        explicit operator Vec3i() const;

        float& x();
        float& y();
        float& z();

        const float& x() const;
        const float& y() const;
        const float& z() const;

        float length() const;
        float lengthSquared() const;

        void normalize();
        Vec3 normalized() const;

        bool operator==(const Vec3& rhs) const;
        bool operator!=(const Vec3& rhs) const;

        static Vec3 Zero();
        static Vec3 One();
        static Vec3 Up();
        static Vec3 Right();
        static Vec3 Forward();

    private:
        glm::vec3 m_Data;
    };

    // Arithmetic
    Vec3 operator+(const Vec3& lhs, const Vec3& rhs);
    Vec3 operator-(const Vec3& lhs, const Vec3& rhs);
    Vec3 operator*(const Vec3& lhs, float rhs);
    Vec3 operator*(float lhs, const Vec3& rhs);
    Vec3 operator/(const Vec3& lhs, float rhs);

    // Math
    float Dot(const Vec3& a, const Vec3& b);
    Vec3 Cross(const Vec3& a, const Vec3& b);



    class Vec3i
    {
    public:
        Vec3i();
        explicit Vec3i(int xyz);
        Vec3i(int x, int y, int z);
        explicit Vec3i(const glm::ivec3&);

        explicit operator const glm::ivec3& () const;
        explicit operator Vec3 () const;

        int& x();
        int& y();
        int& z();

        const int& x() const;
        const int& y() const;
        const int& z() const;

        bool operator==(const Vec3i&) const;
        bool operator!=(const Vec3i&) const;

        static Vec3i Zero();
        static Vec3i One();
        static Vec3i Up();
        static Vec3i Right();
        static Vec3i Forward();

    private:
        glm::ivec3 m_Data;
    };

    Vec3i operator+(const Vec3i&, const Vec3i&);
    Vec3i operator-(const Vec3i&, const Vec3i&);
    Vec3i operator*(const Vec3i&, int);
    Vec3i operator*(int, const Vec3i&);
    Vec3i operator/(const Vec3i&, int);



}


std::ostream& operator<<(std::ostream& os, const qk::Vec3& pos);

std::ostream& operator<<(std::ostream& os, const qk::Vec3i& pos);