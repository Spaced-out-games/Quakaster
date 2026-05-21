#pragma once
#include <Handle.h>

using qk::Handle;

namespace qk::gfx::rhi {


	struct HVertexArray : Handle {};

	HVertexArray VertexArray_ctor();

	void VertexArray_dtor(HVertexArray& hVertexArray);

	void VertexArray_bind(const HVertexArray& hVertexArray);

	bool VertexArray_valid(const HVertexArray& hVertexArray);

}