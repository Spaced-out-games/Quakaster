#pragma once
#include "core.h"
#include "Handle.h"
#include <stdint.h>
#include <vector>



namespace qk::gfx::rhi {




    enum class ScalarType : uint32_t {
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

        ptr
    };

    enum class InputRate: uint8_t {
        Vertex,
        Instance
    };

    // ------------------------------------------------------------
    // Runtime size query (used by layout systems, strides, etc.)
    // ------------------------------------------------------------
    size_t QK_API ScalarType_size(ScalarType type);

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

            // internal tracking for auto-offset computation
            uint32_t m_CurrentOffset[8] = { 0 }; // assumes small binding count for now

        public:
            // ------------------------------------------------------------
            // Add a vertex buffer stream (vertex or instance)
            // ------------------------------------------------------------
            void add_binding(uint32_t binding, uint32_t stride, InputRate rate = InputRate::Vertex);

            // ------------------------------------------------------------
            // Add an attribute to a specific binding
            // Automatically assigns correct offset
            // ------------------------------------------------------------
            void push(const VertexAttribute& attr, uint32_t location, uint32_t binding);

            // ------------------------------------------------------------
            // Query helpers
            // ------------------------------------------------------------
            const std::vector<VertexAttributeBinding>& attributes() const;

            // gets all the bindings
            const std::vector<VertexBufferBinding>& bindings() const;

            // total stride for a binding.
            uint32_t stride(uint32_t binding) const;
    };

    








}

