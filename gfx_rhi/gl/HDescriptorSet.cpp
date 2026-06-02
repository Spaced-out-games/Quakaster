#include "HDescriptorSet.h"
#include <Resource.h>
#include <glad/glad.h>

namespace qk::gfx::rhi {

    struct DescriptorSet_impl : qk::Resource
    {
        DescriptorSet_createinfo descriptors;
    };

    static inline qk::ResourceRegistry<DescriptorSet_impl>
        g_DescriptorSets;

    // ---------------- ctor ----------------

    HDescriptorSet DescriptorSet_ctor(
        const DescriptorSet_createinfo& info)
    {
        DescriptorSet_impl impl;
        impl.descriptors = info;

        auto res = g_DescriptorSets.insert(
            "descriptor_set",
            std::move(impl));
        return res;
    }

    // ---------------- dtor ----------------

    void DescriptorSet_dtor(HDescriptorSet& hDescriptorSet)
    {
        if (!hDescriptorSet)
            return;

        g_DescriptorSets.destroy(hDescriptorSet);
        hDescriptorSet = nullptr;
    }

    // ---------------- bind ----------------

    bool DescriptorSet_bind(HDescriptorSet hDescriptorSet)
    {
        if (!hDescriptorSet)
            return false;

        auto* set = hDescriptorSet;
        auto& descriptors = set->descriptors;

        for (auto& desc : descriptors)
        {
            switch (desc.type)
            {
            case EDescriptorType::Texture:
                if (!Texture_valid(desc.texture.hTex))
                    return false;

                Texture_bind(
                    desc.texture.hTex,
                    desc.binding);
                break;

            case EDescriptorType::UniformBuffer:
                if (!UniformBuffer_valid(desc.uniform.hBuf))
                    return false;

                UniformBuffer_bind(
                    desc.uniform.hBuf,
                    desc.binding);
                break;

            case EDescriptorType::StorageBuffer:
                return false;

            default:
                break;
            }
        }

        return true;
    }

    // ---------------- valid ----------------

    bool DescriptorSet_valid(HDescriptorSet hDescriptorSet)
    {
        if (!hDescriptorSet)
            return false;

        auto& descriptors = hDescriptorSet->descriptors;

        for (auto& desc : descriptors)
        {
            switch (desc.type)
            {
            case EDescriptorType::Texture:
                if (!Texture_valid(desc.texture.hTex))
                    return false;
                break;

            case EDescriptorType::UniformBuffer:
                if (!UniformBuffer_valid(desc.uniform.hBuf))
                    return false;
                break;

            case EDescriptorType::StorageBuffer:
                return false;

            default:
                break;
            }
        }

        return true;
    }

    // ---------------- bind program ----------------

    void DescriptorSet_bindProgram(
        HDescriptorSet hDescriptorSet,
        RShaderProgram hProgram)
    {
        if (!hDescriptorSet)
            return;

        auto& set = hDescriptorSet->descriptors;

        for (const auto& desc : set)
        {
            switch (desc.type)
            {
            case EDescriptorType::Texture:
                if (!Texture_valid(desc.texture.hTex))
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