#pragma once
#include "HCommandBuffer.h" // for PrimitiveTopology
#include "HDescriptorSet.h"
#include <array>
#include "HShader.h"
#include "core.h"

namespace qk::gfx::rhi {



	// Recipe for pipeline creation
	struct Pipeline_recipe {

		std::array<HDescriptorSet, 4> descriptor_sets;
		RShaderProgram				program;
		PrimitiveTopology			topology;
		VertexLayout_recipe			layout;
		//CullMode cull;
		//DepthState depth;
		//BlendState blend;


	};

	// Pipeline implementation
	struct Pipeline_impl;

	// Pipeline handle
	using HPipeline = Pipeline_impl*;

	// Pipeline construction
	HPipeline Pipeline_ctor(const Pipeline_recipe& recipe);

	// Pipeline destruction
	void Pipeline_dtor(HPipeline& pipeline);

	bool Pipeline_bind(const HPipeline& pipeline);




}