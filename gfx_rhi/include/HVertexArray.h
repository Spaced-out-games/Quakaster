#pragma once
#include "core.h"
#include "Resource.h"
#include "Result.h"
#include "HVertexBuffer.h"
#include "HVertexBufferLayout.h"
#include <string>




namespace qk::gfx::rhi {



	struct HVertexArrayTag {};
	using HVertexArray = Resource<HVertexArrayTag>;


	HVertexArray VertexArray_ctor();

	void VertexArray_dtor(HVertexArray& hVertexArray);

	void VertexArray_bind(const HVertexArray& hVertexArray);

	void VertexArray_unbind();


	void VertexArray_applyLayout(const HVertexArray& hVertexArray, const HVertexBuffer& hVertexBuffer, const HVertexBufferLayout& hLayout);


	using VertexArrayRegistry = ResourcePool <HVertexArrayTag, VertexArray_dtor>;


}