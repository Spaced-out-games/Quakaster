#include "HVertexLayout.h"

#include "glad/glad.h"

#include <Resource.h>
#include <vector>

namespace qk::gfx::rhi {

    struct VertexLayout_impl : qk::Resource
    {
        std::vector<VertexAttributeBinding> bindings;
    };

    static inline qk::ResourceRegistry<VertexLayout_impl>
        g_VertexLayouts;

    inline GLenum ScalarType_cast(
        EScalarType type)
    {
        switch (type)
        {
        case EScalarType::i8:
            return GL_BYTE;

        case EScalarType::i16:
            return GL_SHORT;

        case EScalarType::i32:
            return GL_INT;

        case EScalarType::u8:
            return GL_UNSIGNED_BYTE;

        case EScalarType::u16:
            return GL_UNSIGNED_SHORT;

        case EScalarType::u32:
            return GL_UNSIGNED_INT;

        case EScalarType::f32:
            return GL_FLOAT;

        case EScalarType::f64:
            return GL_DOUBLE;

        default:
            return GL_NONE;
        }
    }

    RVertexLayout VertexLayout_ctor(
        str_view label,
        const VertexLayout_recipe& desc)
    {
        VertexLayout_impl impl;

        impl.bindings = desc.bindings;

        return g_VertexLayouts.insert(
            label,
            std::move(impl));
    }

    bool VertexLayout_dtor(
        RVertexLayout& hLayout)
    {
        if (!hLayout)
            return false;

        bool result =
            g_VertexLayouts.destroy(
                hLayout);

        hLayout = nullptr;

        return result;
    }

    bool VertexLayout_valid(
        const RVertexLayout& hLayout)
    {
        return g_VertexLayouts.valid(
            hLayout);
    }

    bool VertexLayout_apply(
        const RVertexLayout& hLayout)
    {
        if (!hLayout)
            return false;

        if (!g_VertexLayouts.valid(
            hLayout))
        {
            return false;
        }

        for (const auto& binding :
            hLayout->bindings)
        {
            for (const auto& attr :
                binding.attributes)
            {
                if (EScalarType_isInteger(
                    attr.component.type) &&
                    !attr.component.normalize)
                {
                    glVertexAttribIPointer(
                        attr.location,
                        attr.component.count,
                        ScalarType_cast(
                            attr.component.type),
                        binding.stride,
                        (const void*)attr.offset);
                }
                else
                {
                    glVertexAttribPointer(
                        attr.location,
                        attr.component.count,
                        ScalarType_cast(
                            attr.component.type),
                        attr.component.normalize,
                        binding.stride,
                        (const void*)attr.offset);
                }

                glEnableVertexAttribArray(
                    attr.location);

                glVertexAttribDivisor(
                    attr.location,
                    binding.input_rate ==
                    InputRate::PerInstance
                    ? 1
                    : 0);
            }
        }

        return true;
    }

}