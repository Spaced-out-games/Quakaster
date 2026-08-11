#pragma once
#include <HBuffer.h>
#include <HCommandBuffer.h>
#include <HDescriptorSet.h>
#include <VertexLayoutDesc.h>
#include <HPipeline.h>
#include <HTexture.h>
#include <HShader.h>
#include "fnv1a.h"
#include <media/Image.h>

using namespace qk::gfx::rhi;

namespace qk::gfx {

	static inline rhi::BufferRegistry		 GBuffers;

	static inline rhi::CommandBufferRegistry CommandBuffers;

	static inline rhi::DescriptorSetRegistry DescriptorSets;

	static inline rhi::PipelineRegistry		 Pipelines;

	static inline rhi::TextureRegistry		 Textures;

	static inline rhi::ShaderProgramRegistry ShaderPrograms;

	inline void destroy_all_resources() {
		CommandBuffers.destroy_everything();
		DescriptorSets.destroy_everything();
		Pipelines.destroy_everything();
		Textures.destroy_everything();
		ShaderPrograms.destroy_everything();
		GBuffers.destroy_everything();
	}

	// note to self: these were not adequately moved to a decent living home. They might not be the best place for these

	inline rhi::HTexture Texture_fromImage(const media::Image& img) {
		HTexture hTex = rhi::Texture_ctor({
			.type = TextureType::TEX_2D,
			.format = TextureFormat::RGB_8,
			.width = img.width(),
			.height = img.height(),
			.depth = 0,
			.mip_levels = 0,
			.array_layers = 0,
			.usage = TextureUsage::Sampled

		});

		if (!hTex) return nullptr;

		rhi::Texture_upload(hTex, {
			.pData = img.data(),
			.format = rhi::TextureFormat::RGBA_8,
			.mip_level = 0,
			.array_layer = 0,
			.width = img.width(),
			.height = img.height(),
			.depth = 0
		});

		rhi::Texture_setSamplingParameters(hTex, {
			.min_filter = rhi::TextureFilter::Nearest,
			.mag_filter = rhi::TextureFilter::Nearest
		});



		return hTex;

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