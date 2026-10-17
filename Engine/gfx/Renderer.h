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
		u32 numInstances = 0;
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

			HTexture load_or_reload_texture(const str& path);

			// unoptimized naive slop for now
			CommandBuffer_recipe compile() const;
			
			template <class ...Args>
			MeshInstance insert(MeshDefinitionID mesh_id, u8 partition, Args&&... args);

			template <class... Args>
			void update(MeshInstance instance, Args&&... args);

			void remove(MeshInstance instance);

			// TODO: add move operation

	};
	// Acts as a `view` to a mesh definition.
	template <class ...SupportedTs>
	class MeshDefinitionBuilder {
		friend class Renderer<SupportedTs...>;

		Renderer<SupportedTs...>& m_Renderer;
		MeshDefinitionBuilder(MeshDefinitionID, Renderer<SupportedTs...>&);
		public:
			MeshDefinitionID m_ID; // temporary

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



			template <class instance_t>
			MeshDefinitionBuilder& pushInstancePool(HVertexBuffer hInstanceBuffer);

			// these probably should be inline free functions referenced by the builder tbh
			MeshDefinitionBuilder& setVertexBufferRange(u32 offset, u32 size);
			MeshDefinitionBuilder& setIndexBufferRange(u32 offset, u32 size);
			MeshDefinitionBuilder& setInstanceCount(u32 count);


			



	};





}



#include "Renderer.inl"