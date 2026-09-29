#pragma once
#include <VertexLayoutDesc.h>
#include <HDescriptorSet.h>
#include "InstancePool.h"
#include <HPipeline.h>
#include <HBuffer.h>


using qk::gfx::rhi::HPipeline;
using qk::gfx::rhi::HDescriptorSet;
using qk::gfx::rhi::HVertexBuffer;
using qk::gfx::rhi::PipelineBinding;
using qk::gfx::rhi::EDescriptorType;

namespace qk::gfx {
	



	// Identifies a pipeline configuration (1-to-1)
	using PipelineDefinitionID = u16;

	// Identifies a VertexBuffer binding group (1-to-many)
	using VertexBufferDefinitionGroupID = u16;

	// Identifies an IndexBuffer binding group (1-to-1)
	using IndexBufferDefinitionID = u16;


	// Identifies a Descriptor Set binding group (1-to-many)
	using DescriptorSetDefinitionGroupID = u16;

	// Uniquely identifies a mesh
	struct MeshConfigurationID {
		PipelineDefinitionID pipeline_id;
		VertexBufferDefinitionGroupID vertexbuffer_group_id;
		IndexBufferDefinitionID indexbuffer_group_id;
		DescriptorSetDefinitionGroupID descriptor_group_id;

		u64 key() const;

		explicit operator u64() const;

		bool operator> (MeshConfigurationID other);

		bool operator==(const MeshConfigurationID& other) const;

	};
	
	
	// Read-only view into mesh details. Ergonomic helper
	struct Mesh_view {
		rhi::PipelineBinding pipeline;
		std::span<rhi::VertexBufferBinding> vertexBufferBindings;
		rhi::IndexBufferBinding indexBufferBinding;
		std::span<rhi::DescriptorSetDefinition> DescriptorSetDefinitions;
		std::span<HVertexBuffer> InstancePools;
		MeshConfigurationID id;

		Mesh_view() {


		}

		Mesh_view(
			rhi::PipelineBinding pipeline,
			std::span<rhi::VertexBufferBinding> vertexBufferBindings,
			rhi::IndexBufferBinding indexBufferBinding,
			std::span<rhi::DescriptorSetDefinition> DescriptorSetDefinitions,
			std::span<HVertexBuffer> InstancePools,
			MeshConfigurationID id
		);

		explicit operator bool() const {
			if (pipeline.hPipeline == nullptr &&
				vertexBufferBindings.empty() &&
				indexBufferBinding.hIndexBuffer == nullptr &&
				DescriptorSetDefinitions.empty()
			) return false;

			return true;
		}

	};
	

	// Stores the state necessary for compiling a frame
	// TODO: 
	// remove the dependency on instance_t, opting for a vector of instancebuffers.
	// Dependency reversal: InstancePool's constructor can take an instancebuffer at creation time and then use that!
	// This lets us do type erasure, however it would complicate the submission process since we don't know the instance_t ahead-of-time.
	// We could store a hash map of typeid -> HVertexBuffer to do validation. This means only one instance_t[] per mesh which is probably
	// fine

	struct MeshCompilationState {
		// public during development

			
		std::unordered_map<PipelineDefinitionID, rhi::PipelineBinding> m_PipelineDefinitions;
		PipelineDefinitionID m_NextPipelineDefinitionID = 0;

		std::unordered_map<VertexBufferDefinitionGroupID, std::vector<rhi::VertexBufferBinding>> m_VertexBufferDefinitions;
		VertexBufferDefinitionGroupID m_NextVertexBufferDefinitionGroupID = 0;

		std::unordered_map<IndexBufferDefinitionID, rhi::IndexBufferBinding> m_IndexBufferDefinitions;
		IndexBufferDefinitionID m_NextIndexBufferDefinitionID = 0;

		std::unordered_map< DescriptorSetDefinitionGroupID, rhi::DescriptorSetDefinition> m_DescriptorSetDefinitions; // FYI: MeshConfigurationID's current definition only supports *one* descriptor set, which needs handled eventually.
		DescriptorSetDefinitionGroupID m_NextDescriptorSetDefinitionGroupID = 0;

		std::unordered_map<MeshConfigurationID, rhi::VertexBufferBinding> m_Instances;
		
		// sorted indexbuffer into m_Instances lets us iterate over them in an orderly fashion when generating the
		// commandbuffer.
		std::vector<u32> m_InstanceProxies;

	public:

		// Define a pipeline
		PipelineDefinitionID define_pipeline(rhi::PipelineBinding pipeline);

		// define a vertexbuffer
		VertexBufferDefinitionGroupID define_vertexbuffer_group(std::span<const rhi::VertexBufferBinding> vertexbuffer_group);

		// define an index buffer
		IndexBufferDefinitionID define_indexbuffer_group(rhi::IndexBufferBinding indexbuffer);

		DescriptorSetDefinitionGroupID define_descriptor_group(const rhi::DescriptorSetDefinition descriptor_group);

		Mesh_view get(MeshConfigurationID id) const;

		bool valid(MeshConfigurationID id) const;

		template <class instance_t>
		void submit(MeshConfigurationID id, const instance_t& initial_value) {

		}







	};
	





}


#include "Mesh.inl"