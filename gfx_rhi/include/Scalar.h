#pragma once
#include <stdint.h>

namespace qk::gfx::rhi {

    enum ScalarType : uint8_t {

        unknown,
        i8,
        i16,
        i32,
        i64,

        u8,
        u16,
        u32,
        u64,

        f32,
        f64,

        ptr,

    };

    enum class ScalarMultiplicity : uint8_t {
        None,
        Scalar,
        Vector,
        Matrix,
    };

    struct ScalarDesc {
        ScalarMultiplicity multiplicity = ScalarMultiplicity::Scalar;
        ScalarType type = f32;
        uint16_t count = 3;
    };

    size_t ScalarType_size(ScalarType type);


}