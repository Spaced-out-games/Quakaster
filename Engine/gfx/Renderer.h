#pragma once
#include <entt/entity/registry.hpp>
#include <HDescriptorSet.h>
#include "InstancePool.h"
#include <HPipeline.h>
#include <HBuffer.h>
#include <utility>
#include <functional>



using qk::gfx::rhi::HBuffer;
using qk::gfx::rhi::HVertexBuffer;
using qk::gfx::rhi::HDescriptorSet;
using qk::gfx::rhi::HPipeline;
using qk::gfx::rhi::HTexture;
using qk::gfx::rhi::HCommandBuffer;
using qk::gfx::rhi::HShaderProgram;
using qk::gfx::rhi::CommandBuffer_recipe;

namespace qk::gfx {

	using MeshComponentPrimitive_t = u64;

	// convenience aliases

	// Defines a pipeline
	using PipelineDefinition = rhi::PipelineBinding;


	// Defines a vertex buffer group
	using VertexBufferGroupDefinition = std::vector<rhi::VertexBufferBinding>;

	// defines an index buffer
	using IndexBufferDefinition = rhi::IndexBufferBinding;

	// defines a group of descriptor sets
	using DescriptorSetGroupDefinition = std::vector<rhi::DescriptorSetDefinition>;


	struct DrawParams {
		// Vertex Buffer state. size == 0 = incomplete draw call.
		u32 VertexBufferOffset = 0;
		u32 VertexBufferSize = 0;

		// Index Buffer State size == 0 = no indexing
		u32 IndexBufferoffset = 0;
		u32 IndexBufferSize = 0;

		// InstanceBuffer state. 0 = no instancing
		//u32 numInstances = 0;
	};

	// when in entity storage, denotes a mesh as having a stale state (pending updates), requiring a flush
	struct CStaleMeshStateTag {
		u8 _;
	};

	// when in entity storage, denotes a mesh as having a stale size (pending updates), requiring a commandbuffer recompile.
	struct CStaleMeshSizeTag {
		u8 _;
	};

	// debug; prevents programmer error by raising an assert if not present.
	struct CMeshFinalizedTag {
		u8 _;
	};

	// Denotes the bounds of a partition for a mesh
	struct MeshPartitionMetaSpan {
		u32 offset = 0;
		u32 size   = 0;
	};

	// Caches partition state for a mesh as a component
	using CMeshPartitionStateCache = std::vector<MeshPartitionMetaSpan>;





	enum class EMeshClass : u8 {
		Static,
		Dynamic
	};



	enum class MeshDefinitionID : u32 {};


	
	enum class MeshPipelineResourceID : u64 {};
	enum class MeshVertexBufferResourceID : u64 {};
	enum class MeshIndexBufferResourceID : u64 {};
	enum class MeshDescriptorSetResourceID: u64 {};


	template <class ...SupportedTs>
	struct MeshDefinitionBuilder;

	struct MeshInstance {
		MeshDefinitionID mesh_ID;
		InstancePoolHandle_t instance;
	};
	
	// tst
	template <class ...SupportedTs>
	struct Renderer {
		public:



		protected: // might change this to private, check the friend status when this compiles as-is
			friend class MeshDefinitionBuilder<SupportedTs>;

			// mesh definitions
			entt::basic_registry<MeshDefinitionID>		m_MeshDefinitions;

			// resource registries
			rhi::BufferRegistry							m_GBuffers;
			rhi::CommandBufferRegistry					m_CommandBuffers;
			rhi::DescriptorSetRegistry					m_DescriptorSets;
			rhi::PipelineRegistry						m_Pipelines;
			rhi::TextureRegistry						m_Textures;
			rhi::ShaderProgramRegistry					m_ShaderPrograms;

		public:


			void flush_mesh(MeshDefinitionID definition);

			void flush_all();

			bool needs_recompiled(MeshDefinitionID definition) const;
			bool needs_recompiled() const;

			bool needs_flushed(MeshDefinitionID definition) const;
			bool needs_flushed() const;




			// todo: add LOD tables into the mix!!! Needed for index buffers

			void destroy_all_resources();

			void define_resource(u64 id, HPipeline hPipeline);

			void define_resource(u64 id, HBuffer hBuffer);

			void define_resource(u64 id, HDescriptorSet hDescriptorSet);

			void define_resource(u64 id, HTexture hTexture);

			void define_resource(u64 id, HCommandBuffer hCmdBuf);

			// maybe make a builder object pattern?

			MeshDefinitionBuilder<SupportedTs...> new_definition();

			template <class T>
			T get_resource(u64 id);

			// not decomposed
			void clear_flags();


			template <>
			HDescriptorSet get_resource<HDescriptorSet>(u64 id) {
				return m_DescriptorSets.peek(id);
			}

			template <>
			HCommandBuffer get_resource<HCommandBuffer>(u64 id) {
				return m_CommandBuffers.peek(id);
			}

			template <>
			HBuffer get_resource<HBuffer>(u64 id) {
				return m_GBuffers.peek(id);
			}

			template <>
			HPipeline get_resource<HPipeline>(u64 id) {
				return m_Pipelines.peek(id);
			}

			
			template <>
			HTexture get_resource<HTexture>(u64 id) {
				return m_Textures.peek(id);
			}

			template <>
			HShaderProgram get_resource<HShaderProgram>(u64 id) {
				return m_ShaderPrograms.peek(id);
			}




			// NOTE: Not completely safe!!! Doesn't check lifetime!
			template <class T>
			void remove_resource(u64 id);


			template <>
			void remove_resource<HDescriptorSet>(u64 id) {
				m_DescriptorSets.try_remove(id);
			}

			// TEST ONLY BAD CODE!!!
			template <>
			void remove_resource<HCommandBuffer>(u64 id) {
				m_CommandBuffers.destroy_everything();
			}

			template <>
			void remove_resource<HBuffer>(u64 id) {
				m_GBuffers.try_remove(id);
			}

			template <>
			void remove_resource<HPipeline>(u64 id) {
				m_Pipelines.try_remove(id);
			}


			template <>
			void remove_resource<HTexture>(u64 id) {
				m_Textures.try_remove(id);
			}

			template <>
			void remove_resource<HShaderProgram>(u64 id) {
				m_ShaderPrograms.try_remove(id);
			}






			HTexture load_or_reload_texture(const str& path);

			// unoptimized naive slop for now
			CommandBuffer_recipe compile() const;
			
			template <class ...Args>
			MeshInstance insert(MeshDefinitionID mesh_id, u8 partition, Args&&... args);

			template <class... Args>
			void update(MeshInstance instance, Args&&... args);

			void remove(MeshInstance instance);

			// TODO: add move operation

			// NOTE: respects the current partition sizes, not the `new` ones
			void syncPartitionCache(MeshDefinitionID target);

			const CMeshPartitionStateCache& LODData(MeshDefinitionID target) const;

			template <class instance_t>
			InstancePool<instance_t>* instancepool(MeshDefinitionID target) {
				return m_MeshDefinitions.try_get<InstancePool<instance_t>>(target);

			}


	};


	// Acts as a `view` to a mesh definition.
	template <class ...SupportedTs>
	class MeshDefinitionBuilder {
		friend class Renderer<SupportedTs...>;

		Renderer<SupportedTs...>& m_Renderer;
		MeshDefinitionBuilder(MeshDefinitionID, Renderer<SupportedTs...>&);
		public:
			MeshDefinitionID m_ID; // temporary
			u8 m_NumLODs = 1;

			// sets the pipeline for this mesh
			MeshDefinitionBuilder& pushPipeline(u64 pipeline_id);

			// pushes a vertex buffer to the group for this mesh
			MeshDefinitionBuilder& pushVertexBuffer(u64 buffer_id, u32 offset, u32 stride, u8 slot);

			// Pushes an index buffer to this mesh.
			// NOTE: default_offset, default_stride are - as the name suggests - default bind state.
			// If this mesh supports LODs via a LOD table, it will use those instead during frame compilation. 
			MeshDefinitionBuilder& pushIndexBuffer(u64 buffer_id, u32 default_offset, u32 default_stride, u8 slot);


			MeshDefinitionBuilder& pushDescriptorSet(u64 id, u8 slot);

			template <class instance_t>
			bool hasInstancePool();


			// NOTE: hInstanceBuffer is assumed to be constructed by the end user but otherwise completely empty. 
			template <class instance_t>
			MeshDefinitionBuilder& pushInstancePool(HVertexBuffer hInstanceBuffer);

			// Sets the mesh's vertex buffer size information
			MeshDefinitionBuilder& setVertexBufferRange(u32 offset, u32 size);

			// Sets the mesh's index buffer size information
			MeshDefinitionBuilder& setIndexBufferRange(u32 offset, u32 size);

			// NOTE: This needs depricated so that instancebuffers can manage it themselves
			MeshDefinitionBuilder& setInstanceCount(u32 count);

			// Allocates a number of LODs
			MeshDefinitionBuilder& setLODCount(u8 num_LODs);

			// static by default. Prevents update() from being called. Not implemented
			MeshDefinitionBuilder& markStatic();


			// static by default. Allows update() to be called. Not implemented
			MeshDefinitionBuilder& markDynamic();


			// Tells the renderer that this mesh is in its final state. You can add and remove instances at this point. Not implemented.
			// In practice, Calls m_Renderer.m_MeshDefinitions.get<InstancePool<true_t>>(m_ID, hInstanceBuffer).add_partition() CMeshPartitionStateCache::size() - 1 times.
			// also, resizes m_Renderer.m_MeshDefinitions.get<CMeshPartitionStateCache>(m_ID); to m_NumLODs. 
			MeshDefinitionID finalize();


			

			


	};




}



#include "Renderer.inl"