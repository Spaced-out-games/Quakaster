#include "HIndexBuffer.h"



#include "rhi_ResourceDefinitions.inl"

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



	HIndexBuffer IndexBuffer_ctor() {
		HIndexBuffer hIndexBuffer;
		GLuint gl_IBO = 0;
		glGenBuffers(1, &gl_IBO);
		set(hIndexBuffer, gl_IBO);
		return hIndexBuffer;
	}

	void IndexBuffer_dtor(HIndexBuffer& hIndexBuffer) {
		QK_ASSERT(hIndexBuffer.valid());

		GLuint gl_IBO = 0;

		glDeleteBuffers(1, &gl_IBO);
		hIndexBuffer = nullptr;
	}

	void IndexBuffer_bind(const HIndexBuffer& hIndexBuffer) {
		QK_ASSERT(hIndexBuffer.valid());

		GLuint gl_IBO = get(hIndexBuffer);
		
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gl_IBO);
	}

	void IndexBuffer_unbind() {
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

	void IndexBuffer_upload(const HIndexBuffer& hIndexBuffer, const void* pIndices, uint32_t count, uint32_t elem_size, BufferUsage usage) {
		QK_ASSERT(hIndexBuffer.valid());
		GLuint gl_IBO = get(hIndexBuffer);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gl_IBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * elem_size, pIndices, BufferUsage_cast(usage));

	}

}