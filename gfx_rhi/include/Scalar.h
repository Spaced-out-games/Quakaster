#pragma once
#include <stdint.h>

namespace qk::gfx::rhi {

    // defines vertex buffer attribute's primitive type
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

    // Determines the multiplicity of a vertex attribute
    enum EScalarMultiplicity : uint8_t {
        None,
        Scalar,
        Vector,
        Matrix,
    };



    // gets the size of a scalar type in bytes
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


    // returns whether or not a scalar is an integral type. 
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



    // EScalarType string conversion for debugging
    inline str_view to_string(EScalarType type) {
        switch (type)
        {
        case EScalarType::unknown: return "???";

        case EScalarType::i8:  return "i8";
        case EScalarType::i16: return "i16";
        case EScalarType::i32: return "i32";
        case EScalarType::i64: return "i64";

        case EScalarType::u8:  return "u8";
        case EScalarType::u16: return "u16";
        case EScalarType::u32: return "u32";
        case EScalarType::u64: return "u64";

        case EScalarType::f32: return "f32";
        case EScalarType::f64: return "f64";

        case EScalarType::ptr: return "ptr";

        default:
            assert(false);
            break;
        }
        return "";
    }



    // Compile-time mappings from C++ type -> EScalarType
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