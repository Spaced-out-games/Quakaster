#pragma once
#include "HVertexBuffer.h"
#include "glad/glad.h"



using qk::gfx::rhi::HVertexBuffer;
using qk::gfx::rhi::HVertexBufferTag;
using qk::set;
using qk::get;



namespace qk {
	template<>
	struct handle_traits<HVertexBufferTag> {
		using value_type = GLuint;
	};

}


namespace qk::gfx::rhi {
	GLuint BufferUsage_cast(BufferUsage usage) {
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
		GLuint hVBO = get(hVertexBuffer);
		QK_ASSERT(hVBO);
		glDeleteBuffers(1, &hVBO);
		hVertexBuffer = nullptr;
	}
	
	void VertexBuffer_bind(const HVertexBuffer& hVertexBuffer) {
		glBindBuffer(GL_ARRAY_BUFFER, get(hVertexBuffer));
	}

	void VertexBuffer_unbind() {
		glBindBuffer(GL_ARRAY_BUFFER, 0);

	}

	void VertexBuffer_upload(const HVertexBuffer& hVertexBuffer, const void* data, size_t count, size_t elem_size, BufferUsage usage) {
		glBindBuffer(GL_ARRAY_BUFFER, get(hVertexBuffer));
		glBufferData(GL_ARRAY_BUFFER, count * elem_size, data, BufferUsage_cast(usage));
	}
}
