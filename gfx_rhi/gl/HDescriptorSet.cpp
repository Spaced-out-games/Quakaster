#include "HDescriptorSet.h"
#include "rhi_globals.h"
#include <glad/glad.h>
#include <Resource.h>
namespace qk::gfx::rhi {


    // NOTE: this is NOT complete!!! It's intended to be a BASIC test with UBOs.
    // this throws a wrench in our plans to do away with RHI-sided registries because
    // we need to be able to resolve buffer names to buffer handles, but this is a terrible design
    // Instead, we could make use of glGetProgramiv, glGetActiveUniformBlockiv, glGetActiveUniformsiv, glGetActiveUniform
    // to invert the problem, or more accurately, forwarding it to OpenGL.

    // Actually, a better approach would be to separate layouts from resource usage.
    // Layouts should be entirely handle and name agnostic.
    // For things that erquire names explicitly in the linking step, we can use program inspection functions to
    // lookup the name of the buffer and link accordingly.
    
    bool DescriptorSet_LNK(const Descriptor& desc, DescriptorSet_impl& impl) {
        //assert(false);
        HBuffer resolved = Buffer_resolve(desc.id);
        impl.descriptors.emplace_back(desc.type, desc.binding, desc.id, UniformBuffer_desc{ resolved });

        return 1;
    }

    bool Descriptor_impl::bind() const {
        switch (type)
        {
        case EDescriptorType::Texture:
            if (!Texture_valid(hTex))
                return false;

            Texture_bind(hTex, binding);
            break;

        case EDescriptorType::UniformBuffer:
            if (!UniformBuffer_valid(uniform.hBuf))
                return false;

            UniformBuffer_bind(uniform.hBuf, binding);
            break;

        case EDescriptorType::StorageBuffer:
            return false;

        default:
            break;
        }
    }



    DescriptorSet_impl::DescriptorSet_impl(const DescriptorSet_createinfo& info) {
        for (const auto& desc : info) {
            if (!DescriptorSet_LNK(desc, *this)) {
                // linker error
            }

            // we'd add to the refcount here.

        }
    }

    // Needs properly implemented. Releases locks
    bool DescriptorSet_impl::destroy() {
        if (!ready_for_deletion())
            return false;

        // we'd decrease refcounts here

    }




    bool DescriptorSet_impl::bind() const {
        for (auto& desc : descriptors)
        {
            desc.bind();
        }
        return true;
    }

    



    HDescriptorSet DescriptorSet_ctor(const DescriptorSet_createinfo& info) {
        return new DescriptorSet_impl(info);
    }


    void DescriptorSet_dtor(HDescriptorSet& hDescriptorSet)
    {
        if (!hDescriptorSet)
            return;

        delete hDescriptorSet;

        hDescriptorSet = nullptr;
    }


    bool DescriptorSet_bind(HDescriptorSet hDescriptorSet)
    {
        if (!hDescriptorSet)
            return false;

        return hDescriptorSet->bind();

    }

    // ---------------- valid ----------------

    bool DescriptorSet_valid(HDescriptorSet hDescriptorSet) {

        return true; // short circuit for now
        
    }

    // ---------------- bind program ----------------

    void DescriptorSet_bindProgram(HDescriptorSet hDescriptorSet, HShaderProgram hProgram)
    {
        if (!hDescriptorSet)
            return;

        auto& set = hDescriptorSet->descriptors;

        for (const auto& desc : set)
        {
            switch (desc.type)
            {
            case EDescriptorType::Texture:
                if (!Texture_valid(desc.hTex))
                    return;

                ShaderProgram_bind_uniform_block(
                    hProgram,
                    desc.id.c_str(),
                    desc.binding);
                break;

            case EDescriptorType::UniformBuffer:
                if (!UniformBuffer_valid(desc.uniform.hBuf))
                    return;

                ShaderProgram_bind_uniform_block(
                    hProgram,
                    desc.id.c_str(),
                    desc.binding);
                break;

            case EDescriptorType::StorageBuffer:
                return;

            default:
                break;
            }
        }
    }

}