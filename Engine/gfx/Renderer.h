#pragma once
#include <entt/entity/registry.hpp>
#include <HDescriptorSet.h>
#include "InstancePool.h"
#include <HPipeline.h>
#include <HBuffer.h>




using qk::gfx::rhi::HBuffer;
using qk::gfx::rhi::HDescriptorSet;
using qk::gfx::rhi::HPipeline;
using qk::gfx::rhi::HTexture;

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

	struct Definition {

	};


	enum class MeshDefinitionID : u32 {};


	
	enum class MeshPipelineResourceID : u64 {};
	enum class MeshVertexBufferResourceID : u64 {};
	enum class MeshIndexBufferResourceID : u64 {};
	enum class MeshDescriptorSetResourceID: u64 {};


	
	struct MeshDefinitionBuilder;


	class Renderer {
		protected:
			friend class MeshDefinitionBuilder;

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
			// todo: add LOD tables into the mix!!! Needed for index buffers

			void destroy_all_resources();

			void define_resource(u64 id, HPipeline hPipeline);

			void define_resource(u64 id, HBuffer hBuffer);

			void define_resource(u64 id, HDescriptorSet hDescriptorSet);


			void define_resource(u64 id, HTexture hTexture);

			// maybe make a builder object pattern?

			MeshDefinitionBuilder new_definition();

	};
	// Acts as a `view` to a mesh definition.
	class MeshDefinitionBuilder {
		friend class Renderer;

		MeshDefinitionID m_ID;
		Renderer& m_Renderer;
		MeshDefinitionBuilder(MeshDefinitionID, Renderer&);
		public:

			// sets the pipeline for this mesh
			MeshDefinitionBuilder& pushPipeline(u64 pipeline_id);

			// pushes a vertex buffer to the group for this mesh
			MeshDefinitionBuilder& pushVertexBuffer(u64 buffer_id, u32 offset, u32 stride, u8 slot);

			// Pushes an index buffer to this mesh.
			// NOTE: default_offset, default_stride are - as the name suggests - default bind state.
			// If this mesh supports LODs via a LOD table, it will use those instead during frame compilation. 
			MeshDefinitionBuilder& pushIndexBuffer(u64 buffer_id, u32 default_offset, u32 default_stride, u8 slot);



	};





}



//#include "Mesh.inl"