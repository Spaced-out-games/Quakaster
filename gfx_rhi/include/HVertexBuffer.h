#pragma once
#include "core.h"
#include "Resource.h"
#include "ResourcePool.h"


namespace qk::gfx::rhi {


	enum BufferUsage {
		StaticDraw,
		DynamicDraw,
		StreamDraw
	};


	struct HVertexBufferTag {};
	using HVertexBuffer = Resource<HVertexBufferTag>;


	HVertexBuffer VertexBuffer_ctor();

	void VertexBuffer_dtor(HVertexBuffer& hVertexBuffer);

	void VertexBuffer_bind(const HVertexBuffer& hVertexBuffer);

	void VertexBuffer_unbind();

	void VertexBuffer_upload(const HVertexBuffer& hVertexBuffer, const void* pData, size_t count, size_t elem_size, BufferUsage usage);


	using VertexBufferRegistry = ResourcePool <HVertexBufferTag, VertexBuffer_dtor>;

}