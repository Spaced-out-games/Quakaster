#pragma once
#include "media/Image.h"
#include "gfx.h"
#include "Renderer.h"
#include "fnv1a.h"

namespace qk::gfx {

	void Renderer::destroy_all_resources() {
		m_CommandBuffers.destroy_everything(); // !!!
		m_DescriptorSets.destroy_everything();
		m_Pipelines.destroy_everything();
		m_Textures.destroy_everything();
		m_ShaderPrograms.destroy_everything(); // !!!
		m_GBuffers.destroy_everything();
	}

	void Renderer::define_resource(u64 id, HBuffer hBuffer) {
		m_GBuffers.insert(id, hBuffer);
	}

	void Renderer::define_resource(u64 id, HDescriptorSet hDescriptorSet) {
		m_DescriptorSets.insert(id, hDescriptorSet);
	}

	void Renderer::define_resource(u64 id, HPipeline hPipeline) {
		m_Pipelines.insert(id, hPipeline);
	}

	void Renderer::define_resource(u64 id, HTexture hTexture) {
		m_Textures.insert(id, hTexture);
	}

	void Renderer::define_resource(u64 id, HCommandBuffer hCmdBuf) {
		m_CommandBuffers.insert(id, hCmdBuf);
	}



	HTexture Renderer::load_or_reload_texture(const str& path) {
		const auto id = qk::string_hash{ path };

		if (auto hTex = m_Textures.peek(id))
			return hTex;

		media::Image img;
		if (!media::load_image(img, path))
			return nullptr;

		rhi::HTexture hTex = Texture_fromImage(img);
		m_Textures.insert(id, hTex);

		return hTex;
	}



	MeshDefinitionBuilder::MeshDefinitionBuilder(MeshDefinitionID id, Renderer& renderer):
	m_ID(id), m_Renderer(renderer) {}


	MeshDefinitionBuilder Renderer::new_definition() {
		return MeshDefinitionBuilder(m_MeshDefinitions.create(), *this);
	}









}