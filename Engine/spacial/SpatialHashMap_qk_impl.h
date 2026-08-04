#pragma once
#include "SpatialHashMap.h"
#include <math/geometric/Vec3.h>
#include <math/geometric/Vec2.h>
#include <functional>

using qk::Vec2;
using qk::Vec2i;

using qk::Vec3;
using qk::Vec3i;
// point hashing implementations
namespace std
{


    template<>
    struct hash<Vec2i>
    {
        size_t operator()(const Vec2i& c) const noexcept {
            return c.x() * 73856093u ^ c.y() * 19349663u;
        }
    };

    template<>
    struct hash<Vec3i>
    {
        size_t operator()(const Vec3i& c) const noexcept {
            return c.x() * 73856093u ^ c.y() * 19349663u ^ c.z() * 83492791u;
        }
    };
}




namespace qk {


    template <>
    struct SpatialTraits<Vec3i, Vec3> {
        static Vec3i to_cell(const Vec3& position, size_t CellSize) {
            return (Vec3i)(position / CellSize);
        }

        class iterator
        {
            Vec3i m_Min;
            Vec3i m_Max;
            Vec3i m_Current;
        public:
            iterator(Vec3i min, Vec3i max): m_Min(min), m_Max(max), m_Current(min) {
            }

            iterator(Vec3i min, Vec3i max, Vec3i current) : m_Min(min), m_Max(max), m_Current(current) {
            }



            Vec3i operator*() const {
                return m_Current;
            }
            iterator& operator++() {


                m_Current.x()++;

                if (m_Current.x() > m_Max.x()) {
                    m_Current.x() = m_Min.x();
                    m_Current.y()++;

                    if (m_Current.y() > m_Max.y()) {
                        m_Current.y() = m_Min.y();
                        m_Current.z()++;
                        
                    }

                }

                return *this;

            }

            bool operator==(const iterator& it) const {
                return m_Current == it.m_Current;
            }
            bool operator!=(const iterator& other) const
            {
                return !(*this == other);
            }   
        };

        static iterator begin(Vec3i min, Vec3i max)
        {
            return iterator(min, max);
        }

        static iterator end(Vec3i min, Vec3i max)
        {
            Vec3i current = min;

            // One-past-the-end sentinel.
            current.z() = max.z() + 1;

            return iterator(min, max, current);
        }

    };

    template <class Value_t, size_t GridScalar>
    using SpatialHashMap3D = SpatialHashMap < Vec3, Vec3i, Value_t, GridScalar> ;


}