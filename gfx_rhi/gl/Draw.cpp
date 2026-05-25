#include "Draw.h"
#include <glad/glad.h>


namespace qk::gfx::rhi {

	inline GLuint PrimitiveType_cast(PrimitiveTopology type) {
		switch (type)
		{
		case qk::gfx::rhi::PrimitiveTopology::Triangle:
			return GL_TRIANGLES;
		case qk::gfx::rhi::PrimitiveTopology::Lines:
			return GL_LINES;
		case qk::gfx::rhi::PrimitiveTopology::Points:
			return GL_POINTS;
		case qk::gfx::rhi::PrimitiveTopology::TriangleStrip:
			return GL_TRIANGLE_STRIP;
		case qk::gfx::rhi::PrimitiveTopology::LineStrip:
			return GL_LINE_STRIP;
		default:
			return GL_TRIANGLES;
			break;
		}
	}



	void DrawArrays(const HVertexArray& hVertexArray, PrimitiveTopology topology, int offset, size_t numVertices) {
		VertexArray_bind(hVertexArray);
		glDrawArrays(PrimitiveType_cast(topology), offset, numVertices);
	}

	void DrawArraysInstanced(const HVertexArray& hVertexArray, PrimitiveTopology topology, int offset, size_t numVertices, size_t numInstances) {
		VertexArray_bind(hVertexArray);
		glDrawArraysInstanced(
			PrimitiveType_cast(topology),
			offset,
			numVertices,
			numInstances
		);
	}

}