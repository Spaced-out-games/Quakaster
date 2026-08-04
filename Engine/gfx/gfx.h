#pragma once
#include <HBuffer.h>
#include <HCommandBuffer.h>
#include <HDescriptorSet.h>
#include <HVertexLayout.h>
#include <HPipeline.h>
#include <HTexture.h>
#include <HShader.h>
#include "fnv1a.h"
#include <media/Image.h>



namespace qk::gfx {

	static inline rhi::BufferRegistry		 GBuffers;

	static inline rhi::CommandBufferRegistry CommandBuffers;

	static inline rhi::DescriptorSetRegistry DescriptorSets;

	static inline rhi::VertexLayoutRegistry  VertexLayouts;

	static inline rhi::PipelineRegistry		 Pipelines;

	static inline rhi::TextureRegistry		 Textures;

	static inline rhi::ShaderProgramRegistry ShaderPrograms;

	inline void destroy_all_resources() {
		CommandBuffers.destroy_everything();
		DescriptorSets.destroy_everything();
		VertexLayouts.destroy_everything();
		Pipelines.destroy_everything();
		Textures.destroy_everything();
		ShaderPrograms.destroy_everything();
		GBuffers.destroy_everything();
	}

	// note to self: these were not adequately moved to a decent living home. They might not be the best place for these

	inline rhi::HTexture Texture_fromImage(const media::Image& img) {
		auto texResult = rhi::Texture_ctor(rhi::TEX_2D);

		if (!texResult) return nullptr;

		rhi::Texture_upload_2D(*texResult, img.data(), img.width(), img.height(), img.channels());

		return *texResult;

	}

	inline void registry_upload(rhi::PipelineRegistry& registry, const std::unordered_map<str, rhi::Pipeline_recipe>& pipelines) {
		rhi::HPipeline hPipeline = nullptr;
		for (const auto& [name, recipe] : pipelines) {
			hPipeline = rhi::Pipeline_ctor(recipe);
			registry.insert(qk::string_hash{ name }, hPipeline);
		}
	}

	inline void registry_upload(rhi::DescriptorSetRegistry& registry, const std::unordered_map<str, rhi::DescriptorSet_recipe>& sets) {
		rhi::HDescriptorSet hDescriptorSet = nullptr;
		for (const auto& [name, recipe] : sets) {
			hDescriptorSet = rhi::DescriptorSet_ctor(recipe);
			registry.insert(qk::string_hash{ name }, hDescriptorSet);
		}
	}

	inline rhi::HTexture load_or_reload(rhi::TextureRegistry& registry, const str& path) {
		const auto id = qk::string_hash{ path };

		if (auto hTex = registry.peek(id))
			return hTex;

		media::Image img;
		if (!media::load_image(img, path))
			return nullptr;

		rhi::HTexture hTex = Texture_fromImage(img);
		registry.insert(id, hTex);

		return hTex;
	}

}