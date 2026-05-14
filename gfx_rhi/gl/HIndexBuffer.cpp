#include "HIndexBuffer.h"



#include "rhi_HandleDefinitions.inl"

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
		GLuint ibo = 0;
		glGenBuffers(1, &ibo);
		set(hIndexBuffer, ibo);
		return hIndexBuffer;
	}

	void IndexBuffer_dtor(HIndexBuffer& hIndexBuffer) {
		GLuint ibo = 0;

		glDeleteBuffers(1, &ibo);

		hIndexBuffer = nullptr;
	}

	void IndexBuffer_bind(const HIndexBuffer& hIndexBuffer) {
		assert(get(hIndexBuffer));
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, get(hIndexBuffer));
	}

	void IndexBuffer_unbind() {
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

	void IndexBuffer_upload(const HIndexBuffer& hIndexBuffer, const void* indices, uint32_t count, uint32_t elem_size, BufferUsage usage) {
		assert(get(hIndexBuffer));

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, get(hIndexBuffer));
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * elem_size, indices, BufferUsage_cast(usage));

	}

}