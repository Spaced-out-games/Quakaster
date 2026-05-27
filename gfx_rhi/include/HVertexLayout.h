#pragma once
#include "VertexLayoutDesc.h"
#include <Handle.h>



namespace qk::gfx::rhi {

	struct HVertexLayout : Handle {};

	HVertexLayout	VertexLayout_ctor(const VertexLayout_createinfo& desc);

	bool			VertexLayout_dtor(const HVertexLayout& hLayout);

	bool			VertexLayout_valid(const HVertexLayout& hLayout);

	// NOTICE: This assumes a VertexArray has already been bound.
	bool			VertexLayout_apply(const HVertexLayout& hLayout);


} // namespace qk::gfx::rhi