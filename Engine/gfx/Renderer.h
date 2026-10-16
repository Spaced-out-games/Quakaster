#pragma once
#include <entt/entity/registry.hpp>
#include <HDescriptorSet.h>
#include "InstancePool.h"
#include <HPipeline.h>
#include <HBuffer.h>
#include <utility>
#include <functional>



using qk::gfx::rhi::HBuffer;
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
		u32 VertexBufferoffset = 0;
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


	template <class ...Ts>
	struct MeshDefinitionBuilder;

	template <class ...Ts>
	struct Renderer {
		public:
			// Flushes all InstancePools for one mesh
			//using MeshFlush_pfn_t = void (*)(const entt::basic_registry<MeshDefinitionID>&, MeshDefinitionID);
			
			// Compiles a mesh
			//using MeshCompile_pfn_t = void(*)(const entt::basic_registry<MeshDefinitionID>&, MeshDefinitionID, CommandBuffer_recipe&);

			// deletes a mesh instance
			//using MeshInstanceDeleter_pfn_t = void(*)(entt::basic_registry<MeshDefinitionID>&, InstancePoolTable_t&, InstancePoolHandle_t);

			//template <class Ts...>
			//using MeshInstanceBuilder_pfn_t = std::function<InstancePoolHandle_t(entt::basic_registry<MeshDefinitionID>&, InstancePoolTable_t&, Ts&&)>;



		protected: // might change this to private, check the friend status when this compiles as-is
			friend class MeshDefinitionBuilder<Ts>;

			// mesh definitions
			entt::basic_registry<MeshDefinitionID>		m_MeshDefinitions;

			// resource registries
			rhi::BufferRegistry							m_GBuffers;
			rhi::CommandBufferRegistry					m_CommandBuffers;
			rhi::DescriptorSetRegistry					m_DescriptorSets;
			rhi::PipelineRegistry						m_Pipelines;
			rhi::TextureRegistry						m_Textures;
			rhi::ShaderProgramRegistry					m_ShaderPrograms;

			//MeshFlush_pfn_t							m_pFlusher = nullptr;
			//MeshCompile_pfn_t						m_pMeshCompiler = nullptr;
			//MeshInstanceDeleter_pfn_t						m_pMeshInstanceDeleter = nullptr;
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

			MeshDefinitionBuilder<Ts...> new_definition();

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
			CommandBuffer_recipe compile() const {
				CommandBuffer_recipe result;
				
				m_MeshDefinitions.each([this](MeshDefinitionID definition) {
					
					// bind the pipeline
					if (const auto* pPipelineDef = m_MeshDefinitions.try_get<PipelineDefinition>(definition)) {
						result.bindPipeline(pPipelineDef->hPipeline);
					} else return; // early exit

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



	};
	// Acts as a `view` to a mesh definition.
	template <class ...Ts>
	class MeshDefinitionBuilder {
		friend class Renderer<Ts>;

		MeshDefinitionID m_ID;
		Renderer<Ts...>& m_Renderer;
		MeshDefinitionBuilder(MeshDefinitionID, Renderer<Ts...>&);
		public:

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
			MeshDefinitionBuilder& pushInstancePool(InstancePool<instance_t>&& pool);





	};





}



#include "Renderer.inl"