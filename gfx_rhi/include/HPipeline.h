#pragma once
#include "HCommandBuffer.h" // for PrimitiveTopology
#include <array>
#include "HShader.h"
#include "core.h"

namespace qk::gfx::rhi {



	// Recipe for pipeline creation
	struct Pipeline_recipe {

		HShaderProgram				program;
		PrimitiveTopology			topology;
		HVertexLayout				layout;
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