#include "VertexBuffer.h"
#include "GL/glew.h"

namespace qk::gfx {


	VertexBuffer_impl::VertexBuffer_impl()
	{


	}

	void VertexBuffer_impl::init_impl() {
		if (!m_Handle) glGenBuffers(1, &m_Handle);
		else __debugbreak();
	}


	void VertexBuffer_impl::upload_impl(const void* data, size_t count, size_t elem_size, unsigned int usage) {
		m_Count = count;
		glBindBuffer(GL_ARRAY_BUFFER, m_Handle);
		glBufferData(GL_ARRAY_BUFFER, count * elem_size, data, usage);
	}

	VertexBuffer_impl::~VertexBuffer_impl()
	{
		if (m_Handle != NULL_HANDLE) {
			glDeleteBuffers(1, &m_Handle);
		}
	}

	void VertexBuffer_impl::bind_impl() const {
		glBindBuffer(GL_ARRAY_BUFFER, m_Handle);
	}
	void VertexBuffer_impl::unbind_impl() const {
		#ifdef _DEBUG
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		#endif
	}

	void VertexBuffer_impl::destroy_impl() {
		if (m_Handle == NULL_HANDLE) return;
		glDeleteBuffers(1, &m_Handle);
		m_Handle = NULL_HANDLE;
	}
	size_t VertexBuffer_impl::count() const {
		return m_Count;
	}



	




} // namespace gfx