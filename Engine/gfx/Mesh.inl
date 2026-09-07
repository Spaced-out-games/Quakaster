


namespace qk::gfx {
	
	template <class instance_t>
	Mesh_view<instance_t>::Mesh_view(
		rhi::PipelineBinding pipeline,
		std::span<const rhi::VertexBufferBinding> vertexBufferBindings,
		rhi::IndexBufferBinding indexBufferBinding,
		std::span<const rhi::DescriptorSetBinding> descriptorSetBindings,
		std::span<const InstancePool<instance_t>> InstancePools,
		MeshID id
	): pipeline(pipeline),
		vertexBufferBindings(vertexBufferBindings),
		indexBufferBinding(indexBufferBinding),
		descriptorSetBindings(descriptorSetBindings),
		InstancePools(InstancePools),
		id(id)
	{}
	








}