#pragma once

#include <vector>
#include <string>
#include <stdint.h>

#include "HTexture.h"
#include "HBuffer.h"
#include "HShader.h"
#include "Result.h"
#include "core.h"

namespace qk::gfx::rhi {

    struct Descriptor_impl;
    struct DescriptorSet_impl;

    enum class EDescriptorType {
        Unknown,
        Texture,
        UniformBuffer,
        StorageBuffer
    };
    // Texture descriptor recipe
    struct Texture_desc {
        HTexture hTex;
    };
    // Uniform buffer descriptor recipe
    struct UniformBuffer_desc {
        RUniformBuffer hBuf;
    };

    // Storage buffer descriptor recipe
    struct StorageBuffer_desc {
        RBuffer hBuf;
    };

    // Descriptor recipe
    struct Descriptor {
        EDescriptorType type = EDescriptorType::Unknown;
        u8 binding = 0;
        str id;
    };

    // Descriptor set recipe
    using DescriptorSet_createinfo = std::vector<Descriptor>;

    // Descriptor set implementation
    struct DescriptorSet_impl;

    // Descriptor set handle
    using HDescriptorSet = DescriptorSet_impl*;

    // Descriptor set constructor
    HDescriptorSet DescriptorSet_ctor(const DescriptorSet_createinfo& info);

    // Descriptor set destructor
    void DescriptorSet_dtor(HDescriptorSet& hDescriptorSet);

    // Descriptor set binding
    bool DescriptorSet_bind(HDescriptorSet hDescriptorSet);

    // Descriptor set handle validity check
    bool DescriptorSet_valid(HDescriptorSet hDescriptorSet);


    //void DescriptorSet_bindProgram(HDescriptorSet hDescriptorSet, RShaderProgram hProgram);

}