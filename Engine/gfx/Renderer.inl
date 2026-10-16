#pragma once
//#include "Renderer.h"
#include "media/Image.h"
#include "gfx.h"
#include "Renderer.h"
#include "fnv1a.h"
#include <stdexcept>

namespace qk::gfx {


	template <class... SupportedTs>
	void Renderer<SupportedTs...>::flush_mesh(MeshDefinitionID definition) {
		([&] {
			if (auto* pPool = m_MeshDefinitions.template try_get<InstancePool<SupportedTs>>(definition)) {
				pPool->flush();
			}
		}(), ...);
	}

	template <class... SupportedTs>
	void Renderer<SupportedTs...>::flush_all()
	{
		m_MeshDefinitions.each([this](auto definition) {
			flush_mesh(definition);
		});
	}




	



	template <class ...SupportedTs>
	void Renderer<SupportedTs...>::destroy_all_resources() {
		m_CommandBuffers.destroy_everything(); // !!!
		m_DescriptorSets.destroy_everything();
		m_Pipelines.destroy_everything();
		m_Textures.destroy_everything();
		m_ShaderPrograms.destroy_everything(); // !!!
		m_GBuffers.destroy_everything();
	}

	template <class ...SupportedTs>
	void Renderer<SupportedTs...>::define_resource(u64 id, HBuffer hBuffer) {
		m_GBuffers.insert(id, hBuffer);
	}

	template <class ...SupportedTs>
	void Renderer<SupportedTs...>::define_resource(u64 id, HDescriptorSet hDescriptorSet) {
		m_DescriptorSets.insert(id, hDescriptorSet);
	}

	template <class ...SupportedTs>
	void Renderer<SupportedTs...>::define_resource(u64 id, HPipeline hPipeline) {
		m_Pipelines.insert(id, hPipeline);
	}

	template <class ...SupportedTs>
	void Renderer<SupportedTs...>::define_resource(u64 id, HTexture hTexture) {
		m_Textures.insert(id, hTexture);
	}

	template <class ...SupportedTs>
	void Renderer<SupportedTs...>::define_resource(u64 id, HCommandBuffer hCmdBuf) {
		m_CommandBuffers.insert(id, hCmdBuf);
	}



	template <class ...SupportedTs>
	HTexture Renderer<SupportedTs...>::load_or_reload_texture(const str& path) {
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




	template <class ...SupportedTs>
	MeshDefinitionBuilder<SupportedTs...> Renderer<SupportedTs...>::new_definition() {

		MeshDefinitionID id = m_MeshDefinitions.create();


		// create the basic entries
		m_MeshDefinitions.emplace<DrawParams>(id);
		m_MeshDefinitions.emplace<InstancePoolTable_t>(id);

		return MeshDefinitionBuilder(id, *this);
	}

	template <class ...SupportedTs>
	CommandBuffer_recipe Renderer<SupportedTs...>::compile() const {
		CommandBuffer_recipe result;

		m_MeshDefinitions.each([this](MeshDefinitionID definition) {

			// bind the pipeline
			if (const auto* pPipelineDef = m_MeshDefinitions.try_get<PipelineDefinition>(definition)) {
				result.bindPipeline(pPipelineDef->hPipeline);
			}
			else return; // early exit

		// then the vertex buffers
		if (const auto* pVertexBufferGroup = m_MeshDefinitions.try_get<VertexBufferGroupDefinition>(definition)) {
			for (const auto& vertexBuffer : *pVertexBufferGroup) {
				result.bindVertexBuffer(vertexBuffer.hVertexBuffer, vertexBuffer.slot, vertexBuffer.offset, vertexBuffer.stride);
			}
		}

		// then the index buffers
		if (const auto* pIndexBufferDefinition = m_MeshDefinitions.try_get<IndexBufferDefinition>(definition)) {
			for (const auto& indexBuffer : *pIndexBufferDefinition) {
				result.bindIndexBuffer(indexBuffer.hIndexBuffer);
			}
		}

		// then the descriptor sets
		if (const auto* pDescriptorSetDefinition = m_MeshDefinitions.try_get<DescriptorSetGroupDefinition>(definition)) {
			for (const auto& descriptorSet : *pDescriptorSetDefinition) {
				result.bindDescriptorSet(descriptorSet.hDescriptorSet);
			}
		}

		// Just use the DrawParams for the draw call
		const auto& drawParams = m_MeshDefinitions.get<DrawParams>(definition);

		if (drawParams.numInstances == 0)
			result.drawArrays(drawParams.offset, drawParams.numVertices);
		else
			result.drawArraysInstanced(drawParams.offset, drawParams.numVertices, drawParams.numInstances);



		});

		return result;

	}

	template <class... SupportedTs>
	template <class... Args>
	MeshInstance Renderer<SupportedTs...>::insert(
		MeshDefinitionID mesh_id,
		u8 partition,
		Args&&... args
	) {
		// Make sure all Args are supported
		static_assert(all_of_Ts_v<
			type_list<std::decay_t<Args>...>,
			type_list<SupportedTs...>
		>);

		// Allocate an instance
		MeshInstance result;
		result.mesh_ID = mesh_id;

		auto& table =
			m_MeshDefinitions.get<InstancePoolTable_t>(mesh_id);

		result.instance = table.insert({});

		// Dispatch to each pool that exists on this mesh definition
		(
			[&] {
			if (auto* pPool =
			m_MeshDefinitions.template try_get<
				InstancePool<std::decay_t<Args>>
			>(mesh_id))
			{
				pPool->insert(
					table,
					std::forward<Args>(args),
					partition,
					result.instance
				);
			}
			else {
				throw std::runtime_error(
					"Mesh definition is missing a required InstancePool"
				);
			}
		}(),
			...
			);

		return result;
	}


	template <class... SupportedTs>
	template <class... Args>
	void Renderer<SupportedTs...>::update(
		MeshInstance instance,
		Args&&... args
	) {
		static_assert(all_of_Ts_v<
			type_list<std::decay_t<Args>...>,
			type_list<SupportedTs...>
		>, "Instance contains an unsupported type");

		auto& table =
			m_MeshDefinitions.get<InstancePoolTable_t>(instance.mesh_ID);

		if (!table.valid(instance.instance)) {
			throw std::runtime_error("Invalid mesh instance handle");
		}

		// Update each corresponding pool.
		(
			[&] {
			using T = std::decay_t<Args>;

		auto* pPool =
			m_MeshDefinitions.template try_get<InstancePool<T>>(
				instance.mesh_ID
				);

		if (!pPool) {
			throw std::runtime_error(
				"Mesh definition is missing a required InstancePool"
			);
		}

		pPool->set(
			table,
			instance.instance,
			args
		);
		}(),
			...
			);
	}

	template <class... SupportedTs>
	void Renderer<SupportedTs...>::remove(MeshInstance instance) {
		auto& table =
			m_MeshDefinitions.get<InstancePoolTable_t>(instance.mesh_ID);

		if (!table.valid(instance.instance)) {
			throw std::runtime_error("Invalid mesh instance handle");
		}

		// Remove the instance from every pool present on this definition.
		(
			[&] {
			if (auto* pPool =
			m_MeshDefinitions.template try_get<
				InstancePool<SupportedTs>
			>(instance.mesh_ID))
			{
				pPool->remove(table, instance.instance);
			}
		}(),
			...
			);

		// Invalidate the handle only after the pools have been cleaned up.
		table.remove(instance.instance);
	}









































	template <class ...SupportedTs>
	MeshDefinitionBuilder<SupportedTs...>::MeshDefinitionBuilder(MeshDefinitionID id, Renderer<SupportedTs...>& renderer) :
		m_ID(id), m_Renderer(renderer) {}

	template <class ...SupportedTs>
	MeshDefinitionBuilder<SupportedTs...>& MeshDefinitionBuilder<SupportedTs...>::pushPipeline(u64 pipeline_id) {
		HPipeline hPipeline = m_Renderer.get_resource<HPipeline>(pipeline_id);
		if (!hPipeline) {
			assert(false);
			return *this;
		}
		m_Renderer.m_MeshDefinitions.emplace_or_replace<PipelineDefinition>(m_ID, hPipeline);
		return *this;
	}

	template <class ...SupportedTs>
	MeshDefinitionBuilder<SupportedTs...>& MeshDefinitionBuilder<SupportedTs...>::pushVertexBuffer(u64 buffer_id, u32 offset, u32 stride, u8 slot) {
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

	template <class ...SupportedTs>
	MeshDefinitionBuilder<SupportedTs...>& MeshDefinitionBuilder<SupportedTs...>::pushIndexBuffer(u64 buffer_id, u32 default_offset, u32 default_stride, u8 slot) {
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


	template <class... SupportedTs>
	template <class instance_t>
	bool MeshDefinitionBuilder<SupportedTs...>::hasInstancePool() {
		return m_Renderer.m_MeshDefinitions
			.all_of<InstancePool<instance_t>>(m_ID);
	}

	template <class... SupportedTs>
	template <class instance_t>
	MeshDefinitionBuilder<SupportedTs...>& MeshDefinitionBuilder<SupportedTs...>::pushInstancePool(InstancePool<instance_t>&& pool) {
		static_assert(is_type_in_list_v<std::decay_t<instance_t>, SupportedTs...>,
			"PoolType is not in the allowed set of types");
		
		if (hasInstancePool<instance_t>()) {
			assert(false && "mesh definition already has a pool for this instance type");
			return *this;
		}

		m_Renderer.m_MeshDefinitions.emplace<InstancePool<std::decay_t<instance_t>>>(m_ID, std::move(pool));
		return *this;
	}

	template <class... SupportedTs>
	MeshDefinitionBuilder<SupportedTs...>& MeshDefinitionBuilder<SupportedTs...>::setVertexBufferRange(u32 offset, u32 size) {
		auto& params = m_Renderer.m_MeshDefinitions.get<DrawParams>(m_ID);
		params.VertexBufferOffset = offset;
		params.VertexBufferSize = size;
		return *this;
	}




	template <class... SupportedTs>
	MeshDefinitionBuilder<SupportedTs...>& MeshDefinitionBuilder<SupportedTs...>::setIndexBufferRange(u32 offset, u32 size) {
		auto& params = m_Renderer.m_MeshDefinitions.get<DrawParams>(m_ID);
		params.IndexBufferOffset = offset;
		params.IndexBufferSize = size;
		return *this;
	}

	template <class... SupportedTs>
	MeshDefinitionBuilder<SupportedTs...>& MeshDefinitionBuilder<SupportedTs...>::setInstanceCount(u32 count) {
		auto& params = m_Renderer.m_MeshDefinitions.get<DrawParams>(m_ID);
		params.numInstances = count;
		return *this;
	}










}