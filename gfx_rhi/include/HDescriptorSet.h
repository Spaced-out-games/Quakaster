#pragma once
#include "HTexture.h"
#include "HBuffer.h"
#define RHI_MAX_DESCRIPTORSETS_BOUND 4

namespace qk::gfx::rhi {


    enum class EDescriptorType {
        Unknown,
        Texture,
        UniformBuffer,
        StorageBuffer
    };

    struct DescriptorBinding {
        EDescriptorType type;
        // bindings are auto-generated sequentially
    };



    using DescriptorSet_recipe = std::vector<DescriptorBinding>;
	struct DescriptorSet_impl;

	using HDescriptorSet = DescriptorSet_impl*;

    HDescriptorSet DescriptorSet_ctor(const DescriptorSet_recipe& recipe);

    void DescriptorSet_dtor(const HDescriptorSet& hDescriptorSet);

    void DescriptorSet_free(HDescriptorSet& hDescriptorSet);

	void DescriptorSet_setResource(HDescriptorSet hDescriptorSet, u32 binding, HTexture hTexture);
	void DescriptorSet_setResource(HDescriptorSet hDescriptorSet, u32 binding, HUniformBuffer hUniformBuffer);

	// this is temporary and should be deprecated ASAP. This is just for bootstrapping a test
	void DescriptorSet_bind(HDescriptorSet hDescriptorSet);

    Resource* Resource_cast(const HDescriptorSet& hDescriptorSet);

    using DescriptorSetRegistry = ResourceRegistry<DescriptorSet_impl, DescriptorSet_dtor, DescriptorSet_free>;


}