#pragma once
#include "core.h"
#include "Resource.h"
#include <stdint.h>
#include <vector>
#include "Scalar.h"


namespace qk::gfx::rhi {

    static constexpr uint32_t AUTO = UINT_MAX;


    enum class InputRate: uint8_t {
        Vertex,
        Instance
    };

    // ------------------------------------------------------------
    // Runtime size query (used by layout systems, strides, etc.)
    // ------------------------------------------------------------
    

    // ------------------------------------------------------------
    // Compile-time mapping from C++ type -> ScalarType
    // ------------------------------------------------------------
    template <typename T>
    constexpr ScalarType ScalarType_cast() {
        return ScalarType::unknown;
    }

    // ------------------------- signed ints ----------------------

    template <>
    constexpr ScalarType ScalarType_cast<int8_t>() {
        return ScalarType::i8;
    }

    template <>
    constexpr ScalarType ScalarType_cast<int16_t>() {
        return ScalarType::i16;
    }

    template <>
    constexpr ScalarType ScalarType_cast<int32_t>() {
        return ScalarType::i32;
    }

    template <>
    constexpr ScalarType ScalarType_cast<int64_t>() {
        return ScalarType::i64;
    }

    // ------------------------- unsigned ints --------------------

    template <>
    constexpr ScalarType ScalarType_cast<uint8_t>() {
        return ScalarType::u8;
    }

    template <>
    constexpr ScalarType ScalarType_cast<uint16_t>() {
        return ScalarType::u16;
    }

    template <>
    constexpr ScalarType ScalarType_cast<uint32_t>() {
        return ScalarType::u32;
    }

    template <>
    constexpr ScalarType ScalarType_cast<uint64_t>() {
        return ScalarType::u64;
    }

    // ------------------------- floats ---------------------------

    template <>
    constexpr ScalarType ScalarType_cast<float>() {
        return ScalarType::f32;
    }

    template <>
    constexpr ScalarType ScalarType_cast<double>() {
        return ScalarType::f64;
    }

    struct VertexAttribute {
        ScalarType type;
        uint32_t count;
        bool normalize;
    };

    struct VertexAttributeBinding {
        VertexAttribute attr;
        uint32_t location;
        uint32_t offset;
        uint32_t binding;
    };

    struct VertexBufferBinding {
        uint32_t stride;
        uint32_t binding;
        InputRate rate;

    };


    class QK_API VertexBufferLayoutDesc {
        private:
            std::vector<VertexAttributeBinding> m_Attributes;
            std::vector<VertexBufferBinding> m_Bindings;

            uint32_t m_LastLocation = AUTO;
            uint32_t m_LastBinding = 0;
        public:

            void add_binding(uint32_t binding, uint32_t stride, InputRate rate = InputRate::Vertex);


            void push(
                ScalarType type, uint32_t count, bool normalize,
                uint32_t location = AUTO, uint32_t binding = AUTO
            );




            const std::vector<VertexAttributeBinding>& attributes() const;


            const std::vector<VertexBufferBinding>& bindings() const;

            uint32_t stride(uint32_t binding) const;
    };

    








}

