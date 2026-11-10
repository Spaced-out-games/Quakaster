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

	//static inline rhi::BufferRegistry		 GBuffers;

	//static inline rhi::CommandBufferRegistry CommandBuffers;

	//static inline rhi::DescriptorSetRegistry DescriptorSets;

	//static inline rhi::PipelineRegistry		 Pipelines;

	//static inline rhi::TextureRegistry		 Textures;

	//static inline rhi::ShaderProgramRegistry ShaderPrograms;

	/*inline void destroy_all_resources() {
		CommandBuffers.destroy_everything();
		DescriptorSets.destroy_everything();
		Pipelines.destroy_everything();
		Textures.destroy_everything();
		ShaderPrograms.destroy_everything();
		GBuffers.destroy_everything();
	}*/

	// note to self: these were not adequately moved to a decent living home. They might not be the best place for these





	HTexture Texture_fromImage(const media::Image& img, TextureFormat upload_format, TextureFormat storage_format);

	inline void registry_upload(rhi::PipelineRegistry& registry, const std::unordered_map<str, rhi::Pipeline_recipe>& pipelines) {
		rhi::HPipeline hPipeline = nullptr;
		for (const auto& [name, recipe] : pipelines) {
			hPipeline = rhi::Pipeline_ctor(recipe);
			registry.insert(qk::string_hash{ name }, hPipeline);
		}
	}

	inline void registry_upload(rhi::DescriptorSetRegistry& registry, const std::unordered_map<str, rhi::DescriptorSetLayout>& sets) {
		rhi::HDescriptorSet hDescriptorSet = nullptr;
		for (const auto& [name, recipe] : sets) {
			hDescriptorSet = rhi::DescriptorSet_ctor(recipe);
			registry.insert(qk::string_hash{ name }, hDescriptorSet);
		}
	}

	inline rhi::HTexture load_or_reload(rhi::TextureRegistry& registry, const str& path) {
		const auto id = qk::string_hash{ path };
		auto hTex = registry.peek(id);
		if (hTex)
			return hTex;

		media::Image img;
		if (!media::load_image(img, path))
			return nullptr;


		if (path.ends_with(".png")) {
			hTex = Texture_fromImage(img, TextureFormat::RGBA_8, TextureFormat::RGBA_8);
		}
		else if (path.ends_with(".jpg")) {
			hTex = Texture_fromImage(img, TextureFormat::RGB_8, TextureFormat::RGB_8);
		}
		else {
			QK_ASSERT(false, "Unsupported image type");
		}

		QK_ASSERT(hTex, "hTex was nullptr");

		registry.insert(id, hTex);

		return hTex;
	}

}