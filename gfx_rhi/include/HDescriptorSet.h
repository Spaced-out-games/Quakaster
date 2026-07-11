#pragma once
#include "Resource.h"
#include "HDescriptorSetLayout.h"

namespace qk::gfx::rhi {
	//TODO: Update GLAD to GL 4.2, maybe 4.3, since we need UBO binding points.
	// The alternative is misshaped descriptor sets in 3.3
	struct DescriptorSet_impl;

	using HDescriptorSet = DescriptorSet_impl*;

	HDescriptorSet DescriptorSet_ctor(const HDescriptorSetLayout& hLayout);

	void DescriptorSet_dtor(HDescriptorSetLayout& hLayout);

}