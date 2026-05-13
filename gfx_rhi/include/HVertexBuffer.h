#pragma once
#include "core.h"
#include "Handle.h"


namespace qk::gfx::rhi {


	enum BufferUsage {
		StaticDraw,
		DynamicDraw,
		StreamDraw
	};


	struct HVertexBufferTag {};
	using HVertexBuffer = Handle<HVertexBufferTag>;
	//using HVertexBufferRef = Ref<HVertexBufferTag>;


	HVertexBuffer VertexBuffer_ctor();

	void VertexBuffer_dtor(HVertexBuffer& hVertexBuffer);

	void VertexBuffer_bind(const HVertexBuffer& hVertexBuffer);

	void VertexBuffer_unbind();

	void VertexBuffer_upload(const HVertexBuffer& hVertexBuffer, const void* data, size_t count, size_t elem_size, BufferUsage usage);



}