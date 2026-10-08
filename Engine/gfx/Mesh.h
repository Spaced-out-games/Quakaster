#pragma once
#include <VertexLayoutDesc.h>
#include <HDescriptorSet.h>
#include "InstancePool.h"
#include <HPipeline.h>
#include <HBuffer.h>
#include <typeindex>


using qk::gfx::rhi::HPipeline;
using qk::gfx::rhi::HDescriptorSet;
using qk::gfx::rhi::HVertexBuffer;
using qk::gfx::rhi::PipelineBinding;
using qk::gfx::rhi::EDescriptorType;

namespace qk::gfx {

	// convenience aliases

	// Defines a pipeline
	using PipelineDefinition = rhi::PipelineBinding;

	// Defines a vertex buffer group
	using VertexBufferGroupDefinition = std::vector<rhi::VertexBufferBinding>;

	// defines an index buffer
	using IndexBufferDefinition = rhi::IndexBufferBinding;

	// defines a group of descriptor sets
	using DescriptorSetGroupDefinition = std::vector<rhi::DescriptorSetDefinition>;

	// defines a set of instance pool groups. Maps a type
	//using InstancePoolGroup = std::unordered_map<std::type_index, std::unique_ptr<InstancePoolBase>>;


	struct InstancePoolGroup {
		std::unordered_map<std::type_index, std::unique_ptr<InstancePoolBase>> m_Storages;



		template <class instance_t>
		bool hasInstancePool() {
			return m_Storages.contains(typeid(instance_t));
		}

		template <class instance_t>
		bool addInstancePool(HVertexBuffer hInstanceBuffer) {
			#ifdef _DEBUG
			assert(!hasInstancePool<instance_t>());
			#else
			if (hasInstancePool<instance_t>()) return false;
			#endif
			std::unique_ptr<InstancePoolBase> pool = std::make_unique<InstancePool<instance_t>>(hInstanceBuffer);

			m_Storages.at(typeid(instance_t)) = std::move(pool);

			return true;

		}

		template <class instance_t>
		InstancePool<instance_t>* getInstancePool() {

			auto it = m_Storages.find(typeid(instance_t));

			if (it == m_Storages.end()) return nullptr;

			return it->second.get();

		}

	};





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

		operator u64() const;

		bool operator> (MeshConfigurationID other);

		bool operator==(const MeshConfigurationID& other) const;




	};

}

template <>
struct std::hash<qk::gfx::MeshConfigurationID> {
	size_t operator()(const qk::gfx::MeshConfigurationID& id) const noexcept {
		return (u64)id;
	}
};

namespace qk::gfx {


	
	
	// Read-only view into mesh details. Ergonomic helper
	struct Mesh_view {

		//todo: delete move and copy constructors to force, to some degree, that they stay in scope

		const PipelineDefinition* const pipeline_definition = nullptr;
		const VertexBufferGroupDefinition* const vertexbuffer_group_definition = nullptr;
		const IndexBufferDefinition* const indexbuffer_definition = nullptr;
		const DescriptorSetGroupDefinition* const descriptorset_group_definition = nullptr;
		const MeshConfigurationID id = { 0,0,0,0 };

		Mesh_view() {


		}

		Mesh_view(
			const PipelineDefinition* const pipeline_def,
			const VertexBufferGroupDefinition* const vertexbuffergroup_def,
			const IndexBufferDefinition* const indexbuffer_def,
			const DescriptorSetGroupDefinition* const descriptorsetgroup_def,
			const MeshConfigurationID mesh_id
		);

		explicit operator bool() const {
			return pipeline_definition && vertexbuffer_group_definition && indexbuffer_definition && descriptorset_group_definition;
		}

	};
	

	// Stores the state necessary for compiling a frame.
	// todo: Type erasure. Basically, we redesign m_Instances to be of signature:
	// std::unordered_map<MeshConfiguration, std::unordered_map<std::type_index, std::unique_ptr<InstancePoolBase>>>
	// If 
	// TODO: insert() needs to be variadic, while update() and remove() really don't need to be.
	// we can runtime assert that the Ts... match the T -> InstancePoolBase*'s lineup
	// TODO: just use entt::registry lmao


	struct MeshCompilationState {
		// public during development



		std::unordered_map<PipelineDefinitionID, PipelineDefinition> m_PipelineDefinitions;
		PipelineDefinitionID m_NextPipelineDefinitionID = 0;

		std::unordered_map<VertexBufferDefinitionGroupID, VertexBufferGroupDefinition> m_VertexBufferDefinitions;
		VertexBufferDefinitionGroupID m_NextVertexBufferDefinitionGroupID = 0;

		std::unordered_map<IndexBufferDefinitionID, IndexBufferDefinition> m_IndexBufferDefinitions;
		IndexBufferDefinitionID m_NextIndexBufferDefinitionID = 0;

		std::unordered_map< DescriptorSetDefinitionGroupID, DescriptorSetGroupDefinition> m_DescriptorSetGroupDefinitions;
		DescriptorSetDefinitionGroupID m_NextDescriptorSetDefinitionGroupID = 0;

		
		std::unordered_map<MeshConfigurationID, InstancePoolGroup> m_InstancePoolGroups;
		
		


	public:

		// Define a pipeline
		PipelineDefinitionID define_pipeline(const PipelineDefinition& pipeline_definition);

		// define a vertexbuffer
		VertexBufferDefinitionGroupID define_vertexbuffer_group(const VertexBufferGroupDefinition& vertexbuffer_group);

		// define an index buffer
		IndexBufferDefinitionID define_indexbuffer_group(IndexBufferDefinition indexbuffer);

		// define a descriptor set grouping
		DescriptorSetDefinitionGroupID define_descriptorset_group(const DescriptorSetGroupDefinition& descriptorset_group);

		Mesh_view get(MeshConfigurationID id) const;

		bool valid(MeshConfigurationID id) const;

		template <class Ts>
		InstancePoolHandle_t add_instance(MeshConfigurationID mesh_id, Ts& instance_data) {
			// invalid mesh configuration
			if (!valid(mesh_id)) return {};

			// placeholder
			return {};
		}







	};
	





}



#include "Mesh.inl"