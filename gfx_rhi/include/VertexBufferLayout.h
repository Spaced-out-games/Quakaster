#pragma once
#include "core.h"
#include "Handle.h"
#include "VertexBufferLayoutDesc.h"


namespace qk::gfx::rhi {

	struct HVertexBufferLayoutTag {};

	using HVertexBufferLayout = qk::Handle<HVertexBufferLayoutTag>;


	HVertexBufferLayout QK_API VertexBufferLayout_ctor(const VertexBufferLayoutDesc& desc);
	void QK_API VertexBufferLayout_dtor(HVertexBufferLayout& hLayout);



}