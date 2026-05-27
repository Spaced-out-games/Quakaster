#pragma once
#include <stdint.h>

namespace qk::gfx::rhi {

    enum class EScalarType : uint8_t {

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

    enum EScalarMultiplicity : uint8_t {
        None,
        Scalar,
        Vector,
        Matrix,
    };


    struct ComponentDesc {
        EScalarMultiplicity comp_multiplicity;
        bool                comp_normalize;
        uint32_t            comp_count;
        EScalarType         comp_type;
    };




















    inline size_t EScalarType_size(EScalarType type)
    {
        switch (type)
        {
        case EScalarType::unknown: return 0;

        case EScalarType::i8:  return 1;
        case EScalarType::i16: return 2;
        case EScalarType::i32: return 4;
        case EScalarType::i64: return 8;

        case EScalarType::u8:  return 1;
        case EScalarType::u16: return 2;
        case EScalarType::u32: return 4;
        case EScalarType::u64: return 8;

        case EScalarType::f32: return 4;
        case EScalarType::f64: return 8;

        case EScalarType::ptr: return sizeof(void*);

        default:
            return 0;
        }
    }

    inline bool EScalarType_isInteger(EScalarType type)
    {
        switch (type)
        {
        case EScalarType::i8:
        case EScalarType::i16:
        case EScalarType::i32:
        case EScalarType::u8:
        case EScalarType::u16:
        case EScalarType::u32:
            return true;

        default:
            return false;
        }
    }

    // ------------------------------------------------------------
    // Compile-time mappings from C++ type -> EScalarType
    // ------------------------------------------------------------
    template <typename T>
    constexpr EScalarType EScalarType_cast() {
        return EScalarType::unknown;
    }

    // ------------------------- signed ints ----------------------

    template <>
    constexpr EScalarType EScalarType_cast<int8_t>() {
        return EScalarType::i8;
    }

    template <>
    constexpr EScalarType EScalarType_cast<int16_t>() {
        return EScalarType::i16;
    }

    template <>
    constexpr EScalarType EScalarType_cast<int32_t>() {
        return EScalarType::i32;
    }

    template <>
    constexpr EScalarType EScalarType_cast<int64_t>() {
        return EScalarType::i64;
    }

    // ------------------------- unsigned ints --------------------

    template <>
    constexpr EScalarType EScalarType_cast<uint8_t>() {
        return EScalarType::u8;
    }

    template <>
    constexpr EScalarType EScalarType_cast<uint16_t>() {
        return EScalarType::u16;
    }

    template <>
    constexpr EScalarType EScalarType_cast<uint32_t>() {
        return EScalarType::u32;
    }

    template <>
    constexpr EScalarType EScalarType_cast<uint64_t>() {
        return EScalarType::u64;
    }

    // ------------------------- floats ---------------------------

    template <>
    constexpr EScalarType EScalarType_cast<float>() {
        return EScalarType::f32;
    }

    template <>
    constexpr EScalarType EScalarType_cast<double>() {
        return EScalarType::f64;
    }


    





}