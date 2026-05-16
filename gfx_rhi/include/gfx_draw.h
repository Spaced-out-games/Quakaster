#pragma once
#include "HVertexArray.h"
#include "core.h"
#include "Resource.h"
#include "Result.h"
#include <string>

// this should get depricated before long

namespace qk::gfx::rhi {
	enum PrimitiveType {
		Triangle,
		Lines,
		Points,
		TriangleStrip,
		LineStrip
	};


	Result<void_t, std::string> DrawArrays(const HVertexArray& hVertexArray, PrimitiveType type, int offset, size_t numVertices);

}