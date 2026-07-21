#pragma once
#include "HTexture.h"
#include "HBuffer.h"
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

	void DescriptorSet_setResource(HDescriptorSet hDescriptorSet, u32 binding, HTexture hTexture);

	void DescriptorSet_setResource(HDescriptorSet hDescriptorSet, u32 binding, HUniformBuffer hUniformBuffer);


	// this is temporary and should be deprecated ASAP. This is just for bootstrapping a test
	void DescriptorSet_bind(HDescriptorSet hDescriptorSet);


	void DescriptorSet_dtor(HDescriptorSet hDescriptorSet);



}