#pragma once
#include "VertexLayoutDesc.h"
#include <Handle.h>
#include <rhi_Resource.h>



namespace qk::gfx::rhi {

	struct HVertexLayout : Handle {};

	/*
	HVertexLayout	VertexLayout_ctor(const VertexLayout_createinfo& desc);

	bool			VertexLayout_dtor(const HVertexLayout& hLayout);

	bool			VertexLayout_valid(const HVertexLayout& hLayout);

	// NOTICE: This assumes a VertexArray has already been bound.
	bool			VertexLayout_apply(const HVertexLayout& hLayout);
	*/



	struct RVertexLayout : Resource{};

	RVertexLayout	VertexLayout_ctor(str_view label, const VertexLayout_createinfo& desc);

	bool			VertexLayout_dtor(const RVertexLayout& hLayout);

	bool			VertexLayout_valid(const RVertexLayout& hLayout);

	// NOTICE: This assumes a VertexArray has already been bound.
	bool			VertexLayout_apply(const RVertexLayout& hLayout);



} // namespace qk::gfx::rhi