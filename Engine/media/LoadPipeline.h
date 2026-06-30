#pragma once
#include "HDescriptorSetLayout.h"
#include "VertexLayoutDesc.h"
#include "HPipeline.h"
#include "HCommandBuffer.h"
#include "HShader.h"
#include "json.h"


namespace qk::media {

	using qk::gfx::rhi::DescriptorSetLayout_recipe;
	using qk::gfx::rhi::VertexAttributeBinding;
	using qk::gfx::rhi::VertexLayout_recipe;
	using qk::gfx::rhi::VertexAttribute;
	using qk::gfx::rhi::Pipeline_recipe;
	using qk::gfx::rhi::ShaderModule;

	using qk::media::json;


	bool VertexAttributeFromJSON(const json& j, VertexAttribute& attribute);

	bool VertexAttributeBindingFromJSON(const json& j, VertexAttributeBinding& binding);

	bool ShaderModuleFromJSON(const json& j, ShaderModule& module);

	bool VertexLayout_recipeFromJSON(const json& j, VertexLayout_recipe& layout);

	bool Pipeline_recipeFromJSON(const json& j, Pipeline_recipe& pipeline);



	bool loadPipelinesFromJSON(
		const json& j,
		std::unordered_map<str, Pipeline_recipe>& pipelines,
		std::unordered_map<str, DescriptorSetLayout_recipe>& descriptor_sets
	);

}