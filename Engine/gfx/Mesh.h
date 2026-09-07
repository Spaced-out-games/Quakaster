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

namespace qk::gfx {
	




	using PipelineID = u16;
	using VertexBufferGroupID = u16;
	using IndexBufferGroupID = u16;
	using DescriptorSetGroupID = u16;

	struct MeshID {
		PipelineID pipeline_id;
		VertexBufferGroupID vertexbuffer_group_id;
		IndexBufferGroupID indexbuffer_group_id;
		DescriptorSetGroupID descriptor_group_id;

		u64 key() const;

		explicit operator u64() const;

		bool operator> (MeshID other);

		bool operator==(const MeshID& other) const;

	};
	
	/**/
	template <class instance_t>
	struct Mesh_view {
		rhi::PipelineBinding pipeline;
		std::span<const rhi::VertexBufferBinding> vertexBufferBindings;
		rhi::IndexBufferBinding indexBufferBinding;
		std::span<const rhi::DescriptorSetBinding> descriptorSetBindings;
		std::span<const InstancePool<instance_t>> InstancePools;
		MeshID id;

		Mesh_view(
			rhi::PipelineBinding pipeline,
			std::span<const rhi::VertexBufferBinding> vertexBufferBindings,
			rhi::IndexBufferBinding indexBufferBinding,
			std::span<const rhi::DescriptorSetBinding> descriptorSetBindings,
			std::span<const InstancePool<instance_t>> InstancePools,
			MeshID id
		);

		explicit operator bool() const {
			if (pipeline.hPipeline == nullptr &&
				vertexBufferBindings.empty() &&
				indexBufferBinding.hIndexBuffer == nullptr &&
				descriptorSetBindings.empty()
			) return false;

			return true;
		}

	};
	

	
	template <class instance_t>
	struct MeshClass {
		// public during development

		std::unordered_map<PipelineID, rhi::PipelineBinding> m_Pipelines;
		PipelineID m_NextPipelineID = 0;

		std::unordered_map<VertexBufferGroupID, std::vector<rhi::VertexBufferBinding>> m_VertexBufferGroups;
		VertexBufferGroupID m_NextVertexBufferGroupID = 0;

		std::unordered_map<IndexBufferGroupID, rhi::IndexBufferBinding> m_IndexBufferGroups;
		IndexBufferGroupID m_NextIndexBufferGroupID = 0;

		std::unordered_map< DescriptorSetGroupID, std::vector<rhi::DescriptorSetBinding>> m_DescriptorSetGroups;
		DescriptorSetGroupID m_NextDescriptorSetGroupID = 0;

		std::unordered_map<MeshID, std::vector<InstancePool<instance_t>>> m_Instances;
		
		// sorted indexbuffer into m_Instances lets us iterate over them in an orderly fashion when generating the
		// commandbuffer.
		std::vector<u32> m_InstanceProxies;

	public:

		// Define a pipeline
		PipelineID define_pipeline(rhi::PipelineBinding pipeline)
		{
			// prevent a double insertion
			for (const auto& [id, existing] : m_Pipelines) {
				if (existing == pipeline)
					return id;
			}

			const PipelineID id = m_NextPipelineID++;
			m_Pipelines.emplace(id, std::move(pipeline));
			return id;
		}

		VertexBufferGroupID define_vertexbuffer_group(std::span<const rhi::VertexBufferBinding> vertexbuffer_group)
		{
			// prevent a double insertion
			for (const auto& [id, existing] : m_VertexBufferGroups) {
				if (existing == vertexbuffer_group)
					return id;
			}

			const VertexBufferGroupID id = m_NextVertexBufferGroupID++;
			m_VertexBufferGroups.emplace(id, std::vector<rhi::VertexBufferBinding>(
				vertexbuffer_group.begin(),
				vertexbuffer_group.end()
				));
			return id;
		}

		IndexBufferGroupID define_indexbuffer_group(rhi::IndexBufferBinding indexbuffer)
		{
			// prevent a double insertion
			for (const auto& [id, existing] : m_IndexBufferGroups) {
				if (existing == indexbuffer)
					return id;
			}

			const IndexBufferGroupID id = m_NextIndexBufferGroupID++;
			m_IndexBufferGroups.emplace(id, std::move(indexbuffer));
			return id;
		}

		DescriptorSetGroupID define_descriptor_group(std::span<const rhi::DescriptorSetBinding> descriptor_group) {
			
			// prevent a double insertion
			for (const auto& [id, existing] : m_DescriptorSetGroups) {
				if (existing == descriptor_group)
					return id;
			}

			const DescriptorSetGroupID id = m_NextDescriptorSetGroupID++;
			m_DescriptorSetGroups.emplace(id, std::vector<rhi::DescriptorSetBinding>(
				descriptor_group.begin(),
				descriptor_group.end()
				));
			return id;
		}

		Mesh_view<instance_t> get(MeshID id) const {
			
			// null view state
			rhi::PipelineBinding Pipeline;
			std::span<const rhi::VertexBufferBinding> VertexBuffers;
			rhi::IndexBufferBinding IndexBuffer;
			std::span<const rhi::DescriptorSetBinding> DescriptorSetGroup;
			std::span<const InstancePool<instance_t>> instancepools;

			// build the view state
			if (auto it = m_Pipelines.find(id.pipeline_id); it != m_Pipelines.end())
				Pipeline = it->second;

			if (auto it = m_VertexBufferGroups.find(id.vertexbuffer_group_id); it != m_VertexBufferGroups.end())
				VertexBuffers = it->second;


			
			if (m_IndexBufferGroups.contains(id.indexbuffer_group_id))
				IndexBuffer = m_IndexBufferGroups.at(id.indexbuffer_group_id);

			if (m_DescriptorSetGroups.contains(id.descriptor_group_id))
				DescriptorSetGroup = m_DescriptorSetGroups.at(id.descriptor_group_id);

			if (m_Instances.contains(id))
				instancepools = m_Instances.at(id);

			return Mesh_view{ Pipeline, VertexBuffers, IndexBuffer, DescriptorSetGroup, instancepools, id };

		}






	};
	





}


#include "Mesh.inl"