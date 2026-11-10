#pragma once
//#include "Renderer.h"
#include "media/Image.h"
#include "gfx.h"
#include "Renderer.h"
#include "fnv1a.h"
#include <stdexcept>
#include <core.h>

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
		for (MeshDefinitionID entity : m_MeshDefinitions.storage<MeshDefinitionID>()) {
			flush_mesh(entity);
		}
	}


	template <class ...SupportedTs>
	void Renderer<SupportedTs...>::clear_flags() {
		for (MeshDefinitionID id : m_MeshDefinitions.view<CStaleMeshStateTag, CStaleMeshSizeTag>()) {
			if (m_MeshDefinitions.all_of<CStaleMeshStateTag>(id)) m_MeshDefinitions.erase<CStaleMeshStateTag>(id);
			if (m_MeshDefinitions.all_of<CStaleMeshSizeTag>(id)) m_MeshDefinitions.erase<CStaleMeshSizeTag>(id);
		}
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

		return load_or_reload(m_Textures, path);
	}

	template <class ...SupportedTs>
	MeshDefinitionBuilder<SupportedTs...> Renderer<SupportedTs...>::new_definition() {

		MeshDefinitionID id = m_MeshDefinitions.create();


		// create the basic entries that are gauranteed to exist
		m_MeshDefinitions.emplace<DrawParams>(id);
		m_MeshDefinitions.emplace<InstancePoolTable_t>(id);
		m_MeshDefinitions.emplace<CMeshPartitionStateCache>(id);
		

		m_MeshDefinitions.emplace<EMeshClass>(id, EMeshClass::Static);

		return MeshDefinitionBuilder(id, *this);
	}



	template <class... SupportedTs>
	CommandBuffer_recipe Renderer<SupportedTs...>::compile() const {
		CommandBuffer_recipe result;

		// NOTE: This needs updated to handle LODs, ideally from back-to-front order so far LODs are generally drawn first
		// NOTE: We would need something like HasSupportedInstancePool() to early-exit

		for (MeshDefinitionID definition : m_MeshDefinitions.view<PipelineDefinition>()) {


			// make sure this mesh is finalized
			QK_ASSERT(m_MeshDefinitions.all_of<CMeshFinalizedTag>(definition), "Mesh was not finalized.");

			const auto* pPipelineDef =
				m_MeshDefinitions.try_get<PipelineDefinition>(definition);

			result.bindPipeline(pPipelineDef->hPipeline);

			// then the vertex buffers
			if (const auto* pVertexBufferGroup = m_MeshDefinitions.try_get<VertexBufferGroupDefinition>(definition)) {
				for (const auto& vertexBuffer : *pVertexBufferGroup) {
					result.bindVertexBuffer(vertexBuffer.hVertexBuffer, vertexBuffer.slot, vertexBuffer.offset, vertexBuffer.stride);
				}
			}

			// then the index buffer
			const auto* pIndexBufferDefinition = m_MeshDefinitions.try_get<IndexBufferDefinition>(definition);
			if (pIndexBufferDefinition)
				result.bindIndexBuffer(pIndexBufferDefinition->hIndexBuffer);



			// then the descriptor sets
			if (const auto* pDescriptorSetDefinition = m_MeshDefinitions.try_get<DescriptorSetGroupDefinition>(definition)) {
				for (const auto& descriptorSet : *pDescriptorSetDefinition) {
					result.bindDescriptorSet(descriptorSet.hDescriptorSet, descriptorSet.slot);
				}
			}

			// Just use the DrawParams for the draw call
			const auto& drawParams = m_MeshDefinitions.get<DrawParams>(definition);

			// temporary fix
			auto& cache = m_MeshDefinitions.get<CMeshPartitionStateCache>(definition);

			QK_ASSERT(cache.size() == 1, "LOD0 only support");
			// LOD 0
			u32 numInstances = cache[0].size;


			// TODO: add a flag to mesh types to denote instancing, then uncomment and adapt
			// assert(!isInstanced || numInstances > 0);
			//assert(numInstances > 0);

			result.drawArraysInstanced(drawParams.VertexBufferOffset, drawParams.VertexBufferSize, numInstances);
		}

		return result;
	}



	

	template <class... SupportedTs>
	template <class... Args>
	MeshInstance Renderer<SupportedTs...>::insert(MeshDefinitionID mesh_id, u8 partition, Args&&... args) {
		// Make sure all Args are supported
		static_assert(contains_all_types_v<type_list<std::decay_t<Args>...>, type_list<SupportedTs...>>);

		// make sure this mesh is finalized
		QK_ASSERT(m_MeshDefinitions.all_of<CMeshFinalizedTag>(mesh_id), "Mesh definition was not finalized");

		// Allocate an instance
		MeshInstance result;
		result.mesh_ID = mesh_id;

		auto& table = m_MeshDefinitions.get<InstancePoolTable_t>(mesh_id);

		result.instance = table.insert({});

		// Dispatch to each pool that exists on this mesh definition
		(
			[&] {

				auto* pPool = m_MeshDefinitions.template try_get<InstancePool<std::decay_t<Args>>>(mesh_id);
				QK_ASSERT(pPool != nullptr, "Mesh definition is missing a InstancePool for an inserted instance-data type.");
				pPool->insert(table, std::forward<Args>(args), partition, result.instance);

			}(), ...
		);


		m_MeshDefinitions.emplace_or_replace<CStaleMeshSizeTag>(mesh_id);


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

		// make sure this mesh is finalized
		QK_ASSERT(m_MeshDefinitions.all_of<CMeshFinalizedTag>(instance.mesh_ID), "Mesh definition was not finalized");

		auto& table =
			m_MeshDefinitions.get<InstancePoolTable_t>(instance.mesh_ID);

		QK_ASSERT(table.valid(instance.instance), "Invalid mesh instance handle");



		// Update each corresponding pool.
		(
			[&] {
			using T = std::decay_t<Args>;

		auto* pPool =
			m_MeshDefinitions.template try_get<InstancePool<T>>(
				instance.mesh_ID
				);

		QK_ASSERT(pPool != nullptr, "Mesh definition is missing a required InstancePool");

		

		pPool->set(
			table,
			instance.instance,
			args
		);
		}(),
			...
			);
		m_MeshDefinitions.emplace_or_replace<CStaleMeshStateTag>(instance.mesh_ID);

	}

	template <class... SupportedTs>
	void Renderer<SupportedTs...>::remove(MeshInstance instance) {
		auto& table =
			m_MeshDefinitions.get<InstancePoolTable_t>(instance.mesh_ID);

		// make sure this mesh is finalized
		QK_ASSERT(m_MeshDefinitions.all_of<CMeshFinalizedTag>(instance.mesh_ID), "Mesh was not finalized.");

		QK_ASSERT(table.valid(instance.instance), "Invalid mesh instance handle");

		// Remove the instance from every pool present on this definition.
		(
			[&] {
				if (auto* pPool = m_MeshDefinitions.template try_get<InstancePool<SupportedTs>>(instance.mesh_ID)) {
					pPool->remove(table, instance.instance);
				}
			}(),
			...
		);

		// Invalidate the handle only after the pools have been cleaned up.
		table.remove(instance.instance);

		m_MeshDefinitions.emplace_or_replace<CStaleMeshSizeTag>(instance.mesh_ID);


	}


	template <class ...SupportedTs>
	MeshDefinitionBuilder<SupportedTs...>::MeshDefinitionBuilder(MeshDefinitionID id, Renderer<SupportedTs...>& renderer) :
		m_ID(id), m_Renderer(renderer) {}

	template <class ...SupportedTs>
	MeshDefinitionBuilder<SupportedTs...>& MeshDefinitionBuilder<SupportedTs...>::pushPipeline(u64 pipeline_id) {
		HPipeline hPipeline = m_Renderer.get_resource<HPipeline>(pipeline_id);

		QK_ASSERT(hPipeline, "hPipeline was nullptr");

		if (!hPipeline) return *this;


		m_Renderer.m_MeshDefinitions.emplace_or_replace<PipelineDefinition>(m_ID, hPipeline);
		return *this;
	}

	template <class ...SupportedTs>
	MeshDefinitionBuilder<SupportedTs...>& MeshDefinitionBuilder<SupportedTs...>::pushVertexBuffer(u64 buffer_id, u32 offset, u32 stride, u8 slot) {
		HVertexBuffer hVertexBuffer = m_Renderer.get_resource<HVertexBuffer>(buffer_id);

		QK_ASSERT(hVertexBuffer != nullptr, "hVertexBuffer was nullptr");

		if (!hVertexBuffer) return *this;

		auto& groups = m_Renderer.m_MeshDefinitions.get_or_emplace<VertexBufferGroupDefinition>(m_ID);

		groups.emplace_back(hVertexBuffer, offset, stride, slot);

		return *this;
	}

	template <class ...SupportedTs>
	MeshDefinitionBuilder<SupportedTs...>& MeshDefinitionBuilder<SupportedTs...>::pushIndexBuffer(u64 buffer_id, u32 default_offset, u32 default_stride, u8 slot) {
		HIndexBuffer hIndexBuffer = m_Renderer.get_resource<HIndexBuffer>(buffer_id);

		QK_ASSERT(hIndexBuffer != nullptr, "hIndexBuffer was nullptr");


		if (!hIndexBuffer) return *this;

		m_Renderer.m_MeshDefinitions.emplace_or_replace<IndexBufferDefinition>(m_ID, IndexBufferBinding{ hIndexBuffer, default_offset, default_stride, slot });


		return *this;
	}

	template <class ...Ts>
	MeshDefinitionBuilder<Ts...>& MeshDefinitionBuilder<Ts...>::pushDescriptorSet(u64 id, u8 slot) {
		HDescriptorSet hDescriptorSet = m_Renderer.get_resource<HDescriptorSet>(id);


		QK_ASSERT(hDescriptorSet, "hDescriptorSet was nullptr");



		if (!hDescriptorSet) return *this;

		// NOTICE TO SELF: Descriptor sets weren't compiling because they are using different storages. These need re-written lol
		DescriptorSetGroupDefinition& bindings = m_Renderer.m_MeshDefinitions.get_or_emplace<DescriptorSetGroupDefinition>(m_ID);

		bindings.emplace_back(hDescriptorSet, slot);

		return *this;
	}



	template <class ...SupportedTs>
	bool Renderer<SupportedTs...>::needs_recompiled(MeshDefinitionID definition) const {
		return m_MeshDefinitions.all_of<CStaleMeshSizeTag>(definition);
	}
	
	template <class ...SupportedTs>
	bool Renderer<SupportedTs...>::needs_recompiled() const {
		return m_MeshDefinitions.view<CStaleMeshSizeTag>().size() != 0;
	}

	template <class ...SupportedTs>
	bool Renderer<SupportedTs...>::needs_flushed(MeshDefinitionID definition) const {
		return m_MeshDefinitions.any_of<CStaleMeshStateTag, CStaleMeshSizeTag>(definition);

	}
	template <class ...SupportedTs>
	bool Renderer<SupportedTs...>::needs_flushed() const {
		bool stale_state = m_MeshDefinitions.view<CStaleMeshStateTag>().size() != 0;
		bool stale_size = m_MeshDefinitions.view<CStaleMeshSizeTag>().size() != 0;
		
		return stale_size || stale_state;

	}

	template <class... SupportedTs>
	void Renderer<SupportedTs...>::syncPartitionCache(MeshDefinitionID target) {
		// get the cache
		auto& cache = m_MeshDefinitions.get<CMeshPartitionStateCache>(target);

		size_t offset = 0;
		size_t cacheIndex = 0;
		bool finished = false;

		// since they should always be co-synced for the same mesh,
		// cache the the first valid instancepool's partitions.
		// Early exit if one of them hits
		([&] {
			if (finished) return;
			if (auto* pPool = m_MeshDefinitions.template try_get<InstancePool<SupportedTs>>(target)) {
				for (size_t i = 0; i < pPool->num_partitions(); ++i) {
					QK_ASSERT(cacheIndex < cache.size(), "cacheIndex was out of bounds");
					const auto& partition = pPool->get_partition(i);

					cache[cacheIndex].offset = offset;
					cache[cacheIndex].size = partition.size();

					offset += partition.capacity();
					++cacheIndex;
				}
				finished = true;
			}
		}(), ...);

		// there shouldn't be any partitions
		if (finished == false) cache.resize(0);

	}






	template <class... SupportedTs>
	template <class instance_t>
	bool MeshDefinitionBuilder<SupportedTs...>::hasInstancePool() {
		return m_Renderer.m_MeshDefinitions
			.all_of<InstancePool<instance_t>>(m_ID);
	}

	template <class... SupportedTs>
	template <class instance_t>
	MeshDefinitionBuilder<SupportedTs...>& MeshDefinitionBuilder<SupportedTs...>::pushInstancePool(HVertexBuffer hInstanceBuffer) {
		using true_t = std::decay_t<instance_t>;
		static_assert(contains_type_v<true_t, type_list<SupportedTs...>>,
			"PoolType is not in the allowed set of types");
		
		QK_ASSERT(!hasInstancePool<true_t>(), "Mesh definition already has a pool for this instance type");

		// don't double-insert. Semantics unclear, so throw.
		if (hasInstancePool<true_t>()) throw MeshDefinitionError::POOL_DOUBLE_INSERT_ERROR;

		m_Renderer.m_MeshDefinitions.emplace<InstancePool<true_t>>(m_ID, hInstanceBuffer);
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

	template <class... SupportedTs>
	MeshDefinitionBuilder<SupportedTs...>& MeshDefinitionBuilder<SupportedTs...>::setLODCount(u8 num_LODs) {
		QK_ASSERT(num_LODs != 0, "Only one LOD currently supported!!!");
		m_NumLODs = num_LODs;


		



		return *this;
	}

	// static by default. Prevents update() from being called. Not implemented
	template <class... SupportedTs>
	MeshDefinitionBuilder<SupportedTs...>& MeshDefinitionBuilder<SupportedTs...>::markStatic() {
		m_Renderer.m_MeshDefinitions.get<EMeshClass>(m_ID) = EMeshClass::Static;
		return *this;
	}


	// static by default. Allows update() to be called. Not implemented
	template <class... SupportedTs>
	MeshDefinitionBuilder<SupportedTs...>& MeshDefinitionBuilder<SupportedTs...>::markDynamic() {
		m_Renderer.m_MeshDefinitions.get<EMeshClass>(m_ID) = EMeshClass::Dynamic;
		return *this;
	}

	template <class... SupportedTs>
	MeshDefinitionID MeshDefinitionBuilder<SupportedTs...>::finalize() {

		// first, add all the partitions to every instancepool

		size_t size = m_NumLODs;

		(
			[&] {
				
				if (auto* pPool = m_Renderer.m_MeshDefinitions.template try_get<InstancePool<SupportedTs>>(m_ID)) {
					for (size_t i = 0; i < size; i++) {
						pPool->add_partition();
					}
				}
			}
		(), ...);

		// could be get_or_emplace but we want the asserts while in development for now.
		m_Renderer.m_MeshDefinitions.get<CMeshPartitionStateCache>(m_ID).resize(m_NumLODs);

		m_Renderer.m_MeshDefinitions.emplace<CMeshFinalizedTag>(m_ID);
		// In practice, Calls m_Renderer.m_MeshDefinitions.get<InstancePool<true_t>>(m_ID, hInstanceBuffer).add_partition() CMeshLODData::partition_sizes.size() - 1 times.
		// also, resizes m_Renderer.m_MeshDefinitions.get<CMeshPartitionStateCache>(m_ID); to m_NumLODs.

		return m_ID;
	}

	template <class ...SupportedTs>
	const CMeshPartitionStateCache& Renderer<SupportedTs...>::LODData(MeshDefinitionID target) const {
		return m_MeshDefinitions.get<CMeshPartitionStateCache>(target);
	}








}