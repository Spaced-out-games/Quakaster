#pragma once
#include "core.h"
#include "Handle.h"
#include <stdint.h>
#include <vector>
#include "Scalar.h"

using qk::Handle;

namespace qk::gfx::rhi {

    constexpr uint32_t AUTO = -1;

    enum class InputRate : uint8_t {
        VertexAttribute,
        InstanceAttribute
    };

    struct VertexBindingDesc {
        uint32_t binding = 0;
        uint32_t stride = 0;
        InputRate input_rate = InputRate::VertexAttribute;
    };


    struct VertexAttributeDesc {
        uint32_t location = 0;
        uint32_t binding = 0;

        EScalarType type = rhi::f32;
        uint32_t num_components = 2;

        bool normalize = false;

        uintptr_t offset = 0;
    };


    struct VertexLayoutDesc {
        std::vector<VertexBindingDesc> bindings;
        std::vector<VertexAttributeDesc> attributes;

        void pushBinding(InputRate rate) {

            bindings.emplace_back(
                bindings.size(),
                0,
                rate
            );

        }


        bool pushAttribute(EScalarType type, uint32_t num_components, bool normalize = false, uint32_t location = 0, uint32_t binding = 0, uintptr_t offset = 0) {
            QK_ASSERT(!bindings.empty());

            attributes.emplace_back(
                location,
                binding,
                type,
                num_components,
                normalize,
                offset
            );


        }
        


    };


    struct HVertexLayout : Handle {};

    HVertexLayout VertexLayout_ctor(const VertexLayoutDesc& desc);

    void VertexLayout_dtor(const HVertexLayout& hLayout);

    void VertexLayout_apply(const HVertexLayout& hLayout);

}