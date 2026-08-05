#pragma once
#include "Vec2.h"
#include "Vec3.h"

namespace qk {

    template <class Position_t>
    struct AABB {
        Position_t min;
        Position_t max;

        [[nodiscard]]
        Position_t center() const {
            return (min + max) * 0.5f;
        }

        [[nodiscard]]
        Position_t extent() const {
            return (max - min) * 0.5f;
        }

        [[nodiscard]]
        Position_t size() const {
            return max - min;
        }

        [[nodiscard]]
        bool contains(const Position_t& point) const {
            return point >= min && point <= max;
        }
        AABB(): min(std::numeric_limits<float>::max()),
                max(std::numeric_limits<float>::lowest())
        {}
        AABB(const Position_t& min, const Position_t& max): min(min), max(max) {}

    };

    using AxisAlignedBoundingBox2D = AABB<Vec2>;
    using AxisAlignedBoundingBox3D = AABB<Vec3>;

    using CAxisAlignedBoundingBox2D = AxisAlignedBoundingBox2D;
    using CAxisAlignedBoundingBox3D = AxisAlignedBoundingBox3D;


}