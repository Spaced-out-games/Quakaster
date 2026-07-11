#include "HVertexLayout.h"

#include "glad/glad.h"

#include <Resource.h>
#include <vector>

#include "rhi_globals.h"

namespace qk::gfx::rhi {


    VertexLayout_impl::VertexLayout_impl(const VertexLayout_recipe& recipe) {

        bindings = recipe.bindings;
    }

    void VertexLayout_impl::destroy() {
        return;
    }

    // TODO: Move this functionality into a free function that takes the layout directly so we don't need to heap-allocate one in the GL implementation.
    // We could hide it in HPipeline.cpp
    bool VertexLayout_impl::apply() const {
        GLuint bindingIndex = 0;
        for (const auto& buffer_binding : bindings) {
            bindingIndex = buffer_binding.slot;
            //GLuint bindingIndex = buffer_binding.binding_index;

            for (const auto& attribute_binding : buffer_binding.attributes) {

                if (EScalarType_isInteger(attribute_binding.type) && attribute_binding.normalize) {
                    glVertexAttribFormat(
                        attribute_binding.location,
                        attribute_binding.count,
                        EScalarType_cast(attribute_binding.type),
                        GL_TRUE,
                        attribute_binding.offset
                    );
                }
                else {
                    glVertexAttribFormat(
                        attribute_binding.location,
                        attribute_binding.count,
                        EScalarType_cast(attribute_binding.type),
                        attribute_binding.normalize ? GL_TRUE : GL_FALSE,
                        attribute_binding.offset
                    );
                }

                // 2. Attach attribute to a BINDING SLOT (not a buffer)
                glVertexAttribBinding(
                    attribute_binding.location,
                    bindingIndex
                );

                // 3. Enable attribute
                glEnableVertexAttribArray(attribute_binding.location);

                // 4. Instancing divisor lives per binding slot (not attribute)
                glVertexBindingDivisor(
                    bindingIndex,
                    InputRate_cast(buffer_binding.input_rate)
                );
            }
        }

        return true;
    }





    // API implementations
    HVertexLayout VertexLayout_ctor(const VertexLayout_recipe& desc) {
        return new VertexLayout_impl(desc);
    }

    bool VertexLayout_dtor(
        HVertexLayout& hLayout)
    {
        if (!hLayout)
            return false;

        delete hLayout;

        hLayout = nullptr;

        return true;
    }

    bool VertexLayout_valid(
        const HVertexLayout& hLayout)
    {
        return hLayout != nullptr;
    }



}