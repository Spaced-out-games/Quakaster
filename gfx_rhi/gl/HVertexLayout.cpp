#include "HVertexLayout.h"

#include "glad/glad.h"

#include <Resource.h>
#include <vector>

#include "rhi_globals.h"

namespace qk::gfx::rhi {




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
                        EScalarType_cast(attr.component.type),
                        binding.stride,
                        (const void*)attr.offset);
                }
                else
                {
                    glVertexAttribPointer(
                        attr.location,
                        attr.component.count,
                        EScalarType_cast(attr.component.type),
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