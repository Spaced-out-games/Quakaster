#pragma once
//#include "Renderer.h"
#include "media/Image.h"
#include "gfx.h"
#include "Renderer.h"
#include "fnv1a.h"

namespace qk::gfx {


	template <class... Ts>
	void Renderer<Ts...>::flush_mesh(MeshDefinitionID definition) {
		([&] {
			if (auto* pPool =
			m_MeshDefinitions.template try_get<InstancePool<Ts>>(definition))
				pPool->flush();
		}(), ...);
	}

	template <class... Ts>
	void Renderer<Ts...>::flush_all()
	{
		m_MeshDefinitions.each([this](auto definition) {
			flush_definition<Ts...>(definition);
		});
	}




	



	template <class ...Ts>
	void Renderer<Ts...>::destroy_all_resources() {
		m_CommandBuffers.destroy_everything(); // !!!
		m_DescriptorSets.destroy_everything();
		m_Pipelines.destroy_everything();
		m_Textures.destroy_everything();
		m_ShaderPrograms.destroy_everything(); // !!!
		m_GBuffers.destroy_everything();
	}

	template <class ...Ts>
	void Renderer<Ts...>::define_resource(u64 id, HBuffer hBuffer) {
		m_GBuffers.insert(id, hBuffer);
	}

	template <class ...Ts>
	void Renderer<Ts...>::define_resource(u64 id, HDescriptorSet hDescriptorSet) {
		m_DescriptorSets.insert(id, hDescriptorSet);
	}

	template <class ...Ts>
	void Renderer<Ts...>::define_resource(u64 id, HPipeline hPipeline) {
		m_Pipelines.insert(id, hPipeline);
	}

	template <class ...Ts>
	void Renderer<Ts...>::define_resource(u64 id, HTexture hTexture) {
		m_Textures.insert(id, hTexture);
	}

	template <class ...Ts>
	void Renderer<Ts...>::define_resource(u64 id, HCommandBuffer hCmdBuf) {
		m_CommandBuffers.insert(id, hCmdBuf);
	}



	template <class ...Ts>
	HTexture Renderer<Ts...>::load_or_reload_texture(const str& path) {
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




	template <class ...Ts>
	MeshDefinitionBuilder<Ts...> Renderer<Ts...>::new_definition() {

		MeshDefinitionID id = m_MeshDefinitions.create();


		// create the basic entries
		m_MeshDefinitions.emplace<DrawParams>(id);

		return MeshDefinitionBuilder(id, *this);
	}


	template <class ...Ts>
	MeshDefinitionBuilder<Ts...>::MeshDefinitionBuilder(MeshDefinitionID id, Renderer<Ts...>& renderer) :
		m_ID(id), m_Renderer(renderer) {}

	template <class ...Ts>
	MeshDefinitionBuilder<Ts...>& MeshDefinitionBuilder<Ts...>::pushPipeline(u64 pipeline_id) {
		HPipeline hPipeline = m_Renderer.get_resource<HPipeline>(pipeline_id);
		if (!hPipeline) {
			assert(false);
			return *this;
		}
		m_Renderer.m_MeshDefinitions.emplace_or_replace<PipelineDefinition>(m_ID, hPipeline);
		return *this;
	}

	template <class ...Ts>
	MeshDefinitionBuilder<Ts...>& MeshDefinitionBuilder<Ts...>::pushVertexBuffer(u64 buffer_id, u32 offset, u32 stride, u8 slot) {
		HVertexBuffer hVertexBuffer = m_Renderer.get_resource<HVertexBuffer>(buffer_id);
		if (!hVertexBuffer) {
			assert(false);
			return *this;
		}

		if (!m_Renderer.m_MeshDefinitions.all_of<VertexBufferGroupDefinition>(m_ID)) {
			m_Renderer.m_MeshDefinitions.emplace<VertexBufferGroupDefinition>(m_ID, VertexBufferBinding{ hVertexBuffer, offset, stride, slot });
		}
		else {
			m_Renderer.m_MeshDefinitions.get<VertexBufferGroupDefinition>(m_ID).emplace_back(hVertexBuffer, offset, stride, slot);
		}

		return *this;
	}

	template <class ...Ts>
	MeshDefinitionBuilder<Ts...>& MeshDefinitionBuilder<Ts...>::pushIndexBuffer(u64 buffer_id, u32 default_offset, u32 default_stride, u8 slot) {
		HIndexBuffer hIndexBuffer = m_Renderer.get_resource<HIndexBuffer>(buffer_id);
		if (!hIndexBuffer) {
			assert(false);
			return *this;
		}

		m_Renderer.m_MeshDefinitions.emplace_or_replace<IndexBufferDefinition>(m_ID, IndexBufferBinding{ hIndexBuffer, default_offset, default_stride, slot });


		return *this;
	}

	template <class ...Ts>
	MeshDefinitionBuilder<Ts...>& MeshDefinitionBuilder<Ts...>::pushDescriptorSet(u64 id, u8 slot) {
		HDescriptorSet hDescriptorSet = m_Renderer.get_resource<HDescriptorSet>(id);

		if (!hDescriptorSet) {
			assert(false);
			return *this;
		}

		if (!m_Renderer.m_MeshDefinitions.all_of<DescriptorSetBindings>(m_ID)) {
			m_Renderer.m_MeshDefinitions.emplace<DescriptorSetBindings>(m_ID, DescriptorSetBindings{ { hDescriptorSet, slot } });
		}
		else {
			m_Renderer.m_MeshDefinitions.get<DescriptorSetBindings>(m_ID).emplace_back(hDescriptorSet, slot);
		}

		return *this;
	}












}