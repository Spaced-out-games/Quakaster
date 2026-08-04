#pragma once

#include <glm/vec2.hpp>
#include "geometric.h"
#include <ostream>

namespace qk
{
    class Vec2
    {
    public:
        Vec2();
        explicit Vec2(float xy);
        Vec2(float x, float y);
        explicit Vec2(const glm::vec2& v);

        explicit operator const glm::vec2& () const;

        float& x();
        float& y();

        const float& x() const;
        const float& y() const;

        float length() const;
        float lengthSquared() const;

        void normalize();
        Vec2 normalized() const;

        bool operator==(const Vec2& rhs) const;
        bool operator!=(const Vec2& rhs) const;

        static Vec2 Zero();
        static Vec2 One();
        static Vec2 Up();
        static Vec2 Right();

    private:
        glm::vec2 m_Data;
    };

    Vec2 operator+(const Vec2&, const Vec2&);
    Vec2 operator-(const Vec2&, const Vec2&);
    Vec2 operator*(const Vec2&, float);
    Vec2 operator*(float, const Vec2&);
    Vec2 operator/(const Vec2&, float);

    float Dot(const Vec2&, const Vec2&);



    class Vec2i
    {
    public:
        Vec2i();
        explicit Vec2i(int xy);
        Vec2i(int x, int y);
        explicit Vec2i(const glm::ivec2& v);

        explicit operator const glm::ivec2& () const;

        int& x();
        int& y();

        const int& x() const;
        const int& y() const;

        bool operator==(const Vec2i&) const;
        bool operator!=(const Vec2i&) const;

        static Vec2i Zero();
        static Vec2i One();
        static Vec2i Up();
        static Vec2i Right();

    private:
        glm::ivec2 m_Data;
    };

    Vec2i operator+(const Vec2i&, const Vec2i&);
    Vec2i operator-(const Vec2i&, const Vec2i&);
    Vec2i operator*(const Vec2i&, int);
    Vec2i operator*(int, const Vec2i&);
    Vec2i operator/(const Vec2i&, int);


}

std::ostream& operator<<(std::ostream& os, const qk::Vec2& pos);

std::ostream& operator<<(std::ostream& os, const qk::Vec2i& pos);