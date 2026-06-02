#pragma once
#include "HCommandBuffer.h" // for PrimitiveTopology
#include "HDescriptorSet.h"
#include "HShader.h"
#include "core.h"

namespace qk::gfx::rhi {




	struct Pipeline_recipe {
		str							program_name;
		PrimitiveTopology			topology;
		VertexLayout_recipe			layout;
		Shader_recipe				frag;
		Shader_recipe				vert;
		std::vector<HDescriptorSet> sets;
		//CullMode cull;
		//DepthState depth;
		//BlendState blend;
	};

	struct Pipeline_impl;

	using HPipeline = Pipeline_impl*;

	HPipeline Pipeline_ctor(const Pipeline_recipe& recipe);



}