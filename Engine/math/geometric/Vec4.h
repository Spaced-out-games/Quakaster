#pragma once

#include <glm/vec4.hpp>
#include "geometric.h"
#include <ostream>
namespace qk
{
    class Vec4
    {
    public:
        Vec4();
        explicit Vec4(float xyzw);
        Vec4(float x, float y, float z, float w);
        explicit Vec4(const glm::vec4& v);

        explicit operator const glm::vec4& () const;

        float& x();
        float& y();
        float& z();
        float& w();

        const float& x() const;
        const float& y() const;
        const float& z() const;
        const float& w() const;

        float length() const;
        float lengthSquared() const;

        void normalize();
        Vec4 normalized() const;

        bool operator==(const Vec4&) const;
        bool operator!=(const Vec4&) const;

        static Vec4 Zero();
        static Vec4 One();

    private:
        glm::vec4 m_Data;
    };



    Vec4 operator+(const Vec4&, const Vec4&);
    Vec4 operator-(const Vec4&, const Vec4&);
    Vec4 operator*(const Vec4&, float);
    Vec4 operator*(float, const Vec4&);
    Vec4 operator/(const Vec4&, float);

    float Dot(const Vec4&, const Vec4&);

    class Vec4i
    {
    public:
        Vec4i();
        explicit Vec4i(int xyzw);
        Vec4i(int x, int y, int z, int w);
        explicit Vec4i(const glm::ivec4&);

        explicit operator const glm::ivec4& () const;

        int& x();
        int& y();
        int& z();
        int& w();

        const int& x() const;
        const int& y() const;
        const int& z() const;
        const int& w() const;

        bool operator==(const Vec4i&) const;
        bool operator!=(const Vec4i&) const;

        static Vec4i Zero();
        static Vec4i One();

    private:
        glm::ivec4 m_Data;
    };

    Vec4i operator+(const Vec4i&, const Vec4i&);
    Vec4i operator-(const Vec4i&, const Vec4i&);
    Vec4i operator*(const Vec4i&, int);
    Vec4i operator*(int, const Vec4i&);
    Vec4i operator/(const Vec4i&, int);


}

std::ostream& operator<<(std::ostream& os, const qk::Vec4& pos);

std::ostream& operator<<(std::ostream& os, const qk::Vec4i& pos);