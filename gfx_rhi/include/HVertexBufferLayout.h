#pragma once
#include "core.h"
#include "Resource.h"
#include "VertexBufferLayoutDesc.h"


namespace qk::gfx::rhi {

	struct HVertexBufferLayoutTag {};

	using HVertexBufferLayout = qk::Resource<HVertexBufferLayoutTag>;
	using HVertexBufferLayoutRef = qk::Ref<HVertexBufferLayoutTag>;


	HVertexBufferLayout QK_API VertexBufferLayout_ctor(const VertexBufferLayoutDesc& desc);
	void QK_API VertexBufferLayout_dtor(HVertexBufferLayout& hLayout);



}