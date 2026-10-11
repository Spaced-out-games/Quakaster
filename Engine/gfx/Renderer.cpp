	#pragma once
	#include "Renderer.h"

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
		MeshDefinitionBuilder::MeshDefinitionBuilder(MeshDefinitionID id, Renderer& renderer):
		m_ID(id), m_Renderer(renderer) {}


		MeshDefinitionBuilder Renderer::new_definition() {
			return MeshDefinitionBuilder(m_MeshDefinitions.create(), *this);
		}






	}