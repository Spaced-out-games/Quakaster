#pragma once
#include <rhi_Resource.h>
//#include <Handle.h>


//using qk::Handle;

namespace qk::gfx::rhi {



	struct HVertexArray : Resource {};
	//struct HVertexArray : Handle {};

	HVertexArray VertexArray_ctor(str_view label);

	void VertexArray_dtor(HVertexArray& hVertexArray);

	void VertexArray_bind(const HVertexArray& hVertexArray);

	bool VertexArray_valid(const HVertexArray& hVertexArray);

}