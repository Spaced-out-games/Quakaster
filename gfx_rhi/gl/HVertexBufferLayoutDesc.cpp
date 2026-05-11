#include "HVertexBufferLayoutDesc.h"
#include <vector>

namespace qk::gfx::rhi {

    size_t QK_API ScalarType_size(ScalarType type) {
        switch (type)
        {
        case qk::gfx::rhi::ScalarType::unknown:
            return 0; break;
        case qk::gfx::rhi::ScalarType::i8:
            return 1; break;
        case qk::gfx::rhi::ScalarType::i16:
            return 2; break;
        case qk::gfx::rhi::ScalarType::i32:
            return 4; break;
        case qk::gfx::rhi::ScalarType::i64:
            return 8; break;
        case qk::gfx::rhi::ScalarType::u8:
            return 1; break;
        case qk::gfx::rhi::ScalarType::u16:
            return 2; break;
        case qk::gfx::rhi::ScalarType::u32:
            return 4; break;
        case qk::gfx::rhi::ScalarType::u64:
            return 8; break;
        case qk::gfx::rhi::ScalarType::f32:
            return 4; break;
        case qk::gfx::rhi::ScalarType::f64:
            return 8; break;
        case qk::gfx::rhi::ScalarType::ptr:
            return sizeof(void*); break;
        default:
            return 0;
            break;
        }
    }


    void VertexBufferLayoutDesc::add_binding(uint32_t binding, uint32_t stride, InputRate rate) {
        m_Bindings.push_back(VertexBufferBinding{
            .stride = stride,
            .binding = binding,
            .rate = rate
            });

        if (binding < 8)
            m_CurrentOffset[binding] = 0;
    }

    void VertexBufferLayoutDesc::push(const VertexAttribute& attr,
        uint32_t location,
        uint32_t binding)
    {
        uint32_t offset = m_CurrentOffset[binding];

        m_Attributes.push_back(VertexAttributeBinding{
            .attr = attr,
            .location = location,
            .offset = offset,
            .binding = binding
            });

        // advance offset
        m_CurrentOffset[binding] += ScalarType_size(attr.type) * attr.count;
    }

    const std::vector<VertexAttributeBinding>& VertexBufferLayoutDesc::attributes() const {
        return m_Attributes;
    }

    const std::vector<VertexBufferBinding>& VertexBufferLayoutDesc::bindings() const {
        return m_Bindings;
    }

    // total stride for a binding (optional helper)
    uint32_t VertexBufferLayoutDesc::stride(uint32_t binding) const {
        for (auto& b : m_Bindings) {
            if (b.binding == binding)
                return b.stride;
        }
        return 0;
    }

}