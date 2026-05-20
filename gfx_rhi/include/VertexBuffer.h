#pragma once
#include <Handle.h>

using qk::Handle;

namespace qk::gfx::rhi {


	struct HVertexBuffer : Handle;

	HVertexBuffer VertexBuffer_ctor();

	HVertexBuffer VertexBuffer_dtor();
}