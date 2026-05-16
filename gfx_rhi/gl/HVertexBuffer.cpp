#pragma once
#include "HVertexBuffer.h"
#include "glad/glad.h"



using qk::gfx::rhi::HVertexBuffer;
using qk::gfx::rhi::HVertexBufferTag;
using qk::set;
using qk::get;



namespace qk {
	template<>
	struct ResourceTraits<HVertexBufferTag> {
		using value_type = GLuint;
	};

}


namespace qk::gfx::rhi {
	inline GLuint BufferUsage_cast(BufferUsage usage) {
		switch (usage)
		{
		case qk::gfx::rhi::BufferUsage::StaticDraw:
			return GL_STATIC_DRAW;
			break;
		case qk::gfx::rhi::BufferUsage::DynamicDraw:
			return GL_DYNAMIC_DRAW;
			break;
		case qk::gfx::rhi::BufferUsage::StreamDraw:
			return GL_STREAM_DRAW;
			break;
		default:
			break;
		}
	}




	HVertexBuffer VertexBuffer_ctor() {
		HVertexBuffer hVertexBuffer;
		GLuint hVBO = 0;
		glGenBuffers(1, &hVBO);
		set(hVertexBuffer, hVBO);
		return hVertexBuffer;
	}


	

	void VertexBuffer_dtor(HVertexBuffer& hVertexBuffer) {
		QK_ASSERT(hVertexBuffer.valid());

		GLuint gl_vbo = get(hVertexBuffer);
		glDeleteBuffers(1, &gl_vbo);
		hVertexBuffer = nullptr;
	}
	
	void VertexBuffer_bind(const HVertexBuffer& hVertexBuffer) {
		QK_ASSERT(hVertexBuffer.valid());
		glBindBuffer(GL_ARRAY_BUFFER, get(hVertexBuffer));
	}

	void VertexBuffer_unbind() {
		glBindBuffer(GL_ARRAY_BUFFER, 0);

	}

	void VertexBuffer_upload(const HVertexBuffer& hVertexBuffer, const void* pData, size_t count, size_t elem_size, BufferUsage usage) {
		QK_ASSERT(hVertexBuffer.valid());
		glBindBuffer(GL_ARRAY_BUFFER, get(hVertexBuffer));
		glBufferData(GL_ARRAY_BUFFER, count * elem_size, pData, BufferUsage_cast(usage));
	}
}
