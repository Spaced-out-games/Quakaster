#pragma once
#include "HVertexArray.h"
#include "core.h"
#include "Handle.h"
#include "Result.h"
#include <string>



namespace qk::gfx::rhi {
	enum class PrimitiveType {
		Triangle,
		Lines,
		Points,
		TriangleStrip,
		LineStrip
	};


	Result<void, std::string> DrawArrays(const HVertexArray& hVertexArray, PrimitiveType type, int first, size_t count);

}