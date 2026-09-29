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
		rhi::PipelineBinding pipeline,
		std::span<rhi::VertexBufferBinding> vertexBufferBindings,
		rhi::IndexBufferBinding indexBufferBinding,
		std::span<rhi::DescriptorSetDefinition> DescriptorSetDefinitions,
		std::span<HVertexBuffer> InstancePools,
		MeshConfigurationID id
	) : pipeline(pipeline),
		vertexBufferBindings(vertexBufferBindings.begin(), vertexBufferBindings.end()),
		indexBufferBinding(indexBufferBinding),
		DescriptorSetDefinitions(DescriptorSetDefinitions.begin(), DescriptorSetDefinitions.end()),
		InstancePools(InstancePools.begin(), InstancePools.end()),
		id(id)
	{}

	// Mesh Compilation State
	PipelineDefinitionID MeshCompilationState::define_pipeline(rhi::PipelineBinding pipeline)
	{
		// prevent a double insertion
		for (const auto& [id, existing] : m_PipelineDefinitions) {
			if (existing.hPipeline == pipeline.hPipeline)
				return id;
		}

		const PipelineDefinitionID id = m_NextPipelineDefinitionID++;
		m_PipelineDefinitions.emplace(id, std::move(pipeline));
		return id;
	}

	VertexBufferDefinitionGroupID MeshCompilationState::define_vertexbuffer_group(std::span<const rhi::VertexBufferBinding> vertexbuffer_group)
	{
		// prevent a double insertion
		for (const auto& [id, existing] : m_VertexBufferDefinitions) {
			if (existing.size() == vertexbuffer_group.size())
				return id;
		}

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
		for (const auto& [id, existing] : m_IndexBufferDefinitions) {
			if (existing == indexbuffer)
				return id;
		}

		const IndexBufferDefinitionID id = m_NextIndexBufferDefinitionID++;
		m_IndexBufferDefinitions.emplace(id, std::move(indexbuffer));
		return id;
	}

	DescriptorSetDefinitionGroupID MeshCompilationState::define_descriptor_group(const rhi::DescriptorSetDefinition descriptor_group) {

		// prevent a double insertion
		for (const auto& [id, existing] : m_DescriptorSetDefinitions) {
			if (existing.hDescriptorSet == descriptor_group.hDescriptorSet && existing.resources == descriptor_group.resources)
				return id;
		}
		// get the id
		const DescriptorSetDefinitionGroupID id = m_NextDescriptorSetDefinitionGroupID++;

		// add the descriptor group
		m_DescriptorSetDefinitions.emplace(id, descriptor_group);
		return id;
	}

	Mesh_view MeshCompilationState::get(MeshConfigurationID id) const {

		// null view state
		//Mesh_view view;

		return Mesh_view{};

		/*
		// build the view state
		if (auto it = m_PipelineDefinitions.find(id.pipeline_id); it != m_PipelineDefinitions.end())
			Pipeline = it->second;

		if (auto it = m_VertexBufferDefinitions.find(id.vertexbuffer_group_id); it != m_VertexBufferDefinitions.end())
			VertexBuffers = it->second;



		if (m_IndexBufferDefinitions.contains(id.indexbuffer_group_id))
			IndexBuffer = m_IndexBufferDefinitions.at(id.indexbuffer_group_id);

		if (m_DescriptorSetDefinitions.contains(id.descriptor_group_id))
			DescriptorSetGroup = m_DescriptorSetDefinitions.at(id.descriptor_group_id);

		if (m_Instances.contains(id))
			instancepools = m_Instances;

		return Mesh_view{ Pipeline, VertexBuffers, IndexBuffer, DescriptorSetGroup, instancepools, id };*/

	}


	bool MeshCompilationState::valid(MeshConfigurationID id) const {



		// build the view state
		//if (auto it = m_PipelineDefinitions.find(id.pipeline_id); it == m_PipelineDefinitions.end())
		//	return false;

		//if (auto it = m_VertexBufferDefinitions.find(id.vertexbuffer_group_id); it == m_VertexBufferDefinitions.end())
		//	return false;



		//if (!m_IndexBufferDefinitions.contains(id.indexbuffer_group_id))
		//	return false;

		//if (!m_DescriptorSetDefinitions.contains(id.descriptor_group_id))
		//	return false;

		//if (m_Instances.contains(id))
		//	return false;

		return true;

	}


}