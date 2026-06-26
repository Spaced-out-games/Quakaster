#pragma once
#include "core.h"
#include <vector>
#include "rhi_Resource.h"

namespace qk::gfx::rhi {

    enum class EDescriptorType {
        Unknown,
        Texture,
        UniformBuffer,
        StorageBuffer
    };

    struct DescriptorBinding {
        EDescriptorType type;
        u8              binding;
    };



    using DescriptorSetLayout_recipe = std::vector<DescriptorBinding>;


    struct DescriptorSetLayout_impl;

    using HDescriptorSetLayout = DescriptorSetLayout_impl*;

    HDescriptorSetLayout DescriptorSetLayout_ctor(const DescriptorSetLayout_recipe& recipe);

    void DescriptorSetLayout_dtor(HDescriptorSetLayout& hLayout);


}