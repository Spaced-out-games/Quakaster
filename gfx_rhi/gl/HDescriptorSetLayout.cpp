#include "HDescriptorSetLayout.h"
#include "rhi_globals.h"

namespace qk::gfx::rhi {

	HDescriptorSetLayout DescriptorSetLayout_ctor(const DescriptorSetLayout_recipe& recipe) {
		HDescriptorSetLayout pLayout = new DescriptorSetLayout_impl;

		pLayout->bindings = recipe;

		return pLayout;
	}

	void DescriptorSetLayout_dtor(HDescriptorSetLayout& hLayout) {
		if (!hLayout) return;
		delete hLayout;

	}

}