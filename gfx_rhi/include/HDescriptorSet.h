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

    struct Texture_desc {
        HTexture hTex;
    };

    struct UniformBuffer_desc {
        RUniformBuffer hBuf;
    };

    struct StorageBuffer_desc {
        RBuffer hBuf;
    };

    struct Descriptor {
        EDescriptorType type;
        u8 binding = 0;
        str id;

        union {
            Texture_desc texture;
            UniformBuffer_desc uniform;
            StorageBuffer_desc storage;
        };

        Descriptor() : type(EDescriptorType::Unknown), binding(0) {}
    };

    using DescriptorSet_createinfo = std::vector<Descriptor>;

    struct DescriptorSet_impl;

    using HDescriptorSet = DescriptorSet_impl*;

    HDescriptorSet DescriptorSet_ctor(const DescriptorSet_createinfo& info);
    void DescriptorSet_dtor(HDescriptorSet& hDescriptorSet);

    bool DescriptorSet_bind(HDescriptorSet hDescriptorSet);
    bool DescriptorSet_valid(HDescriptorSet hDescriptorSet);

    void DescriptorSet_bindProgram(HDescriptorSet hDescriptorSet, RShaderProgram hProgram);

}