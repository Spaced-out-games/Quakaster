#pragma once
#include "core.h"
#include "Handle.h"
#include "Result.h"
#include <string>




namespace qk::gfx::rhi {

	struct HVertexArrayTag {};
	using HVertexArray = Handle<HVertexArrayTag>;
	using HVertexArrayRef = Ref<HVertexArrayTag>;


	HVertexArray VertexArray_ctor();

	void VertexArray_dtor(HVertexArray& hdl);

	void VertexArray_bind(const HVertexArray& hdl);

	void VertexArray_unbind();

	HVertexArrayRef VertexArray_get_current_bound() noexcept;





}