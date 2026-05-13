#pragma once
#include "HShaderUniform.h"
#include "core.h"
#include "Handle.h"
#include "HShaderProgram.h"
#include <string>
#include "Result.h"
#include "Scalar.h"

using namespace qk;

namespace qk::gfx::rhi {




	struct HDescriptorSetTag {};
	using HDescriptorSet = Handle<HDescriptorSetTag>;

	HDescriptorSet DescriptorSet_ctor();

	void DescriptorSet_dtor(HDescriptorSet& hDescriptorSet);

	void DescriptorSet_push(HDescriptorSet& hDescriptorSet, HShaderUniform hLocation, ScalarType type,
		uint16_t scalar_count, ScalarMultiplicity multiplicity,
		void* data);

	void DescriptorSet_update(HShaderProgram& hShaderProgram, HDescriptorSet& hDescriptorSet, HShaderUniform& hLocation);



	void DescriptorSet_update(HShaderProgram& hShaderProgram, HDescriptorSet& hDescriptorSet);



}