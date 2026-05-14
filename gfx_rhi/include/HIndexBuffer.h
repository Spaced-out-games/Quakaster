#pragma once
#include "core.h"
#include "Handle.h"


namespace qk::gfx::rhi {





	struct HIndexBufferTag {};
	using HIndexBuffer = Handle<HIndexBufferTag>;
	//using HIndexBufferRef = Ref<HIndexBufferTag>;


	HIndexBuffer IndexBuffer_ctor();

	void IndexBuffer_dtor(HIndexBuffer& hIndexBuffer);

	void IndexBuffer_bind(const HIndexBuffer& hIndexBuffer);

	void IndexBuffer_unbind();

	void IndexBuffer_upload(const HIndexBuffer& hIndexBuffer, const void* data, size_t count, size_t elem_size);



}