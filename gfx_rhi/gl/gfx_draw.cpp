#include "gfx_draw.h"
#include <glad/glad.h>


namespace qk::gfx::rhi {

	inline GLuint PrimitiveType_cast(PrimitiveType type) {
		switch (type)
		{
		case qk::gfx::rhi::PrimitiveType::Triangle:
			return GL_TRIANGLES;
		case qk::gfx::rhi::PrimitiveType::Lines:
			return GL_LINES;
		case qk::gfx::rhi::PrimitiveType::Points:
			return GL_POINTS;
		case qk::gfx::rhi::PrimitiveType::TriangleStrip:
			return GL_TRIANGLE_STRIP;
		case qk::gfx::rhi::PrimitiveType::LineStrip:
			return GL_LINE_STRIP;
		default:
			break;
		}
	}



	Result<void_t, std::string> DrawArrays(const HVertexArray& hVertexArray, PrimitiveType type, int offset, size_t numVertices) {
		VertexArray_bind(hVertexArray);
		glDrawArrays(PrimitiveType_cast(type), offset, numVertices);
		return void_t{};
	}

}