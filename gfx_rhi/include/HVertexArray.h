#pragma once
#include "core.h"
#include "Handle.h"
#include "Result.h"
#include "HVertexBuffer.h"
#include "HVertexBufferLayout.h"
#include <string>




namespace qk::gfx::rhi {



	struct HVertexArrayTag {};
	using HVertexArray = Handle<HVertexArrayTag>;


	HVertexArray VertexArray_ctor();

	void VertexArray_dtor(HVertexArray& hVertexArray);

	void VertexArray_bind(const HVertexArray& hVertexArray);

	void VertexArray_unbind();


	void VertexArray_applyLayout(const HVertexArray& hVertexArray, const HVertexBuffer& hVertexBuffer, const HVertexBufferLayout& hLayout);





}