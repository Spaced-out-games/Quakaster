#include "IndexBuffer.h"

namespace qk::gfx {
	IndexBuffer_impl::IndexBuffer_impl() {}

	void IndexBuffer_impl::init_impl() {
		if (!m_Handle) glGenBuffers(1, &m_Handle);
		else __debugbreak();
	}


	void IndexBuffer_impl::upload_impl(const void* indices, uint32_t count, uint32_t elem_size) {
		m_Count = count;
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Handle);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * elem_size, indices, GL_STATIC_DRAW);
	}




	void IndexBuffer_impl::bind_impl() const {
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Handle);
	}

	void IndexBuffer_impl::unbind_impl() const {
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

	uint32_t IndexBuffer_impl::size_impl(uint32_t elem_size) const {
		return m_Count * elem_size;
	}

	Handle IndexBuffer_impl::handle_impl() {
		return m_Handle;
	}

	Handle IndexBuffer_impl::current_bound() noexcept {
		GLint h = 0;
		glGetIntegerv(GL_ELEMENT_ARRAY_BUFFER_BINDING, &h);
		return static_cast<Handle>(h);
	}
	void IndexBuffer_impl::destroy_impl() {
		if(m_Handle == NULL_HANDLE) return;
		glDeleteBuffers(1, &m_Handle);
		m_Handle = NULL_HANDLE;
	}
	IndexBuffer_impl::~IndexBuffer_impl() {
		destroy_impl();
	}

}