#pragma once
#include "HVertexArray.h"
// this is just a demo surface





namespace qk::gfx::rhi {

	enum PrimitiveTopology {
		Triangle,
		Lines,
		Points,
		TriangleStrip,
		LineStrip
	};

	void DrawArrays(const HVertexArray& hVertexArray, PrimitiveTopology topology, int offset, size_t numVertices);

	void DrawArraysInstanced(const HVertexArray& hVertexArray, PrimitiveTopology topology, int offset, size_t numVertices, size_t numInstances);

}

