#pragma once
#include <algorithm>
#include <core.h>
#include <Vec3.h>
#include <Vec4.h>

namespace qk {

    struct Color {
        u8 r = 255;
        u8 g = 255;
        u8 b = 255;
        u8 a = 255;

        operator Vec3() const {
            return Vec3{
                f32(r) / 255.0f,
                f32(g) / 255.0f,
                f32(b) / 255.0f
            };
        }

        operator Vec4() const {
            return Vec4{
                f32(r) / 255.0f,
                f32(g) / 255.0f,
                f32(b) / 255.0f,
                f32(a) / 255.0f
            };
        }

        Color() = default;

        Color(u8 r, u8 g, u8 b, u8 a = 255)
            : r(r), g(g), b(b), a(a) {}

        Color(f32 r, f32 g, f32 b, f32 a = 1.0f)
            : r(u8(std::clamp(r, 0.0f, 1.0f) * 255.0f)),
            g(u8(std::clamp(g, 0.0f, 1.0f) * 255.0f)),
            b(u8(std::clamp(b, 0.0f, 1.0f) * 255.0f)),
            a(u8(std::clamp(a, 0.0f, 1.0f) * 255.0f)) {}

        explicit Color(const Vec3& v) : Color(v.r, v.g, v.b) {}
        explicit Color(const Vec4& v): Color(v.r, v.g, v.b, v.a) {}
    };

}