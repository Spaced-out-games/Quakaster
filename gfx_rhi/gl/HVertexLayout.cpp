#include "HVertexLayout.h"

#include "glad/glad.h"

#include <Resource.h>
#include <vector>

#include "rhi_globals.h"

namespace qk::gfx::rhi {


    VertexLayout_impl::VertexLayout_impl(const VertexLayout_recipe& recipe) {

        bindings = recipe.bindings;

        // we should PROBABLY ALSO VALIDATE DESCRIPTORS AND LOCK THEM HERE!!!
    }

    bool VertexLayout_impl::destroy() {
        // release locks on descriptors
        return true;
    }


    bool VertexLayout_impl::apply() const {

        for (const auto& binding : bindings) {
            for (const auto& attr : binding.attributes) {
                if (EScalarType_isInteger(attr.component.type) && attr.component.normalize) {
                    glVertexAttribIPointer(
                        attr.location, attr.component.count,
                        EScalarType_cast(attr.component.type), binding.stride,
                        (const void*)attr.offset
                    );
                }
                else
                {
                    glVertexAttribPointer(
                        attr.location,
                        attr.component.count,
                        EScalarType_cast(attr.component.type),
                        attr.component.normalize,
                        binding.stride,
                        (const void*)attr.offset
                    );
                }

                glEnableVertexAttribArray(
                    attr.location
                );

                glVertexAttribDivisor (
                    attr.location,
                    InputRate_cast(binding.input_rate)
                );
            }
        }

        return true;
    }





    // API implementations

    RVertexLayout VertexLayout_ctor(const VertexLayout_recipe& desc) {
        return new VertexLayout_impl(desc);
    }

    bool VertexLayout_dtor(
        RVertexLayout& hLayout)
    {
        if (!hLayout)
            return false;

        delete hLayout;

        hLayout = nullptr;

        return true;
    }

    bool VertexLayout_valid(
        const RVertexLayout& hLayout)
    {
        return hLayout != nullptr;
    }

    bool VertexLayout_apply(
        const RVertexLayout& hLayout)
    {
        if (!hLayout)
            return false;


        return hLayout->apply();
    }

}