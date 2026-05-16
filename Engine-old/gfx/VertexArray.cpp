#include "VertexArray.h"
#include "spdlog/spdlog.h"

namespace qk::gfx {
	VertexArray::VertexArray()
	{
	}
	void VertexArray::init() {
		if (m_Handle == NULL_HANDLE) {
			glGenVertexArrays(1, &m_Handle);
		}
		else {
			spdlog::warn("Attempted to double-initialize a vertex array!");
		}

	}

	void VertexArray::destroy() {
		if (m_Handle != NULL_HANDLE) return;
		glDeleteVertexArrays(1, &m_Handle);
		m_Handle = NULL_HANDLE;
	}


	VertexArray::~VertexArray() {
		destroy();
	}
	void VertexArray::bind() const {
		glBindVertexArray(m_Handle);
	}
	void VertexArray::unbind() const {
		glBindVertexArray(0);
	}
	Handle VertexArray::handle() const {
		return m_Handle;
	}
	VertexArray::operator Handle() const noexcept {
		return m_Handle;
	}

	Handle VertexArray::current_bound() noexcept {
		GLint h = 0;
		glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &h);
		return static_cast<Handle>(h);
	}
	
	void VertexArray::apply_impl(const VertexBufferLayout& layout, VertexBufferLayoutCursor cursor) const {
		unsigned int location = 0;
		unsigned int offset = 0;

		for (auto& elem : layout.m_Elements) {
			glEnableVertexAttribArray(location);

			glVertexAttribPointer(
				location,
				elem.count,
				elem.type,
				elem.normalized,
				layout.m_Stride,
				(void*)(uintptr_t)offset
			);

			offset += elem.count * get_attribute_size(elem.type);
			location++;
		}
	}

}