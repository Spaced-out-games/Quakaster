#include "Mesh.h"


// Mesh ID
namespace qk::gfx {

	// MeshConfigurationID
	
	u64 MeshConfigurationID::key() const {
		return (u64(pipeline_id) << 48) |
			(u64(vertexbuffer_group_id) << 32) |
			(u64(indexbuffer_group_id) << 16) |
			u64(descriptor_group_id);
	}

	MeshConfigurationID::operator u64() const {
		return key();
	}

	bool MeshConfigurationID::operator> (MeshConfigurationID other) {
		return key() > other.key();
	}
	bool MeshConfigurationID::operator==(const MeshConfigurationID& other) const {
		return key() == other.key();
	}

	// Mesh View
	Mesh_view::Mesh_view(
		const PipelineDefinition* const pipeline_def,
		const VertexBufferGroupDefinition* const vertexbuffergroup_def,
		const IndexBufferDefinition* const indexbuffer_def,
		const DescriptorSetGroupDefinition* const descriptorsetgroup_def,
		const MeshConfigurationID mesh_id
	):
		pipeline_definition(pipeline_def),
		vertexbuffer_group_definition(vertexbuffergroup_def),
		indexbuffer_definition(indexbuffer_def),
		descriptorset_group_definition(descriptorsetgroup_def),
		id(mesh_id)
	
	
	{}

	// Mesh Compilation State
	PipelineDefinitionID MeshCompilationState::define_pipeline(const PipelineDefinition& pipeline)
	{
		// prevent a double insertion
		//for (const auto& [id, existing] : m_PipelineDefinitions) {
		//	if (existing.hPipeline == pipeline.hPipeline)
		//		return id;
		//}

		const PipelineDefinitionID id = m_NextPipelineDefinitionID++;
		m_PipelineDefinitions.emplace(id, std::move(pipeline));
		return id;
	}

	VertexBufferDefinitionGroupID MeshCompilationState::define_vertexbuffer_group(const VertexBufferGroupDefinition& vertexbuffer_group)
	{
		// prevent a double insertion
		//for (const auto& [id, existing] : m_VertexBufferDefinitions) {
		//	if (existing.size() == vertexbuffer_group.size())
		//		return id;
		//}

		const VertexBufferDefinitionGroupID id = m_NextVertexBufferDefinitionGroupID++;
		m_VertexBufferDefinitions.emplace(id, std::vector<rhi::VertexBufferBinding>(
			vertexbuffer_group.begin(),
			vertexbuffer_group.end()
			));
		return id;
	}

	IndexBufferDefinitionID MeshCompilationState::define_indexbuffer_group(rhi::IndexBufferBinding indexbuffer)
	{
		// prevent a double insertion
		//for (const auto& [id, existing] : m_IndexBufferDefinitions) {
		//	if (existing == indexbuffer)
		//		return id;
		//}

		const IndexBufferDefinitionID id = m_NextIndexBufferDefinitionID++;
		m_IndexBufferDefinitions.emplace(id, std::move(indexbuffer));
		return id;
	}

	DescriptorSetDefinitionGroupID MeshCompilationState::define_descriptorset_group(const DescriptorSetGroupDefinition& descriptorset_group) {

		// prevent a double insertion
		/*
		
		for (const auto& [id, existing] : m_DescriptorSetDefinitions) {
			if (existing.hDescriptorSet == descriptor_group.hDescriptorSet && existing.resources == descriptor_group.resources)
				return id;
		}
		
		*/

		// get the id
		const DescriptorSetDefinitionGroupID id = m_NextDescriptorSetDefinitionGroupID++;

		// add the descriptor set group
		m_DescriptorSetGroupDefinitions.emplace(id, descriptorset_group);
		return id;
	}

	Mesh_view MeshCompilationState::get(MeshConfigurationID id) const {

		return {}; // for now

	}


	bool MeshCompilationState::valid(MeshConfigurationID id) const {


		// validate the pipeline ID
		if (auto it = m_PipelineDefinitions.find(id.pipeline_id); it == m_PipelineDefinitions.end())
			return false;

		// validate the vertex buffer group ID
		if (auto it = m_VertexBufferDefinitions.find(id.vertexbuffer_group_id); it == m_VertexBufferDefinitions.end())
			return false;


		// validate the index buffer ID
		if (!m_IndexBufferDefinitions.contains(id.indexbuffer_group_id))
			return false;

		// validate the descriptor set group ID
		if (!m_DescriptorSetGroupDefinitions.contains(id.descriptor_group_id))
			return false;
		// NOTICE: This currently causes C2064 to trigger because MeshConfigurationID doesn't have a std::hash overload defined, so it's trying to use the u64 cast operator to no avail. 
		if (m_InstancePoolGroups.contains(id))
			return false;

		return true;

	}


}