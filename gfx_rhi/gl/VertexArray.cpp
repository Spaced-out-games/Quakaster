#include "VertexArray.h"
#include "glad\glad.h"

using qk::gfx::rhi::HVertexArray;
using qk::gfx::rhi::HVertexArrayTag;

namespace qk {

	template<>
	inline GLuint get<HVertexArrayTag, GLuint>(const Handle<HVertexArrayTag>& handle) {
		return (GLuint)handle.m_Handle;
	}

	template<>
	inline void set<HVertexArrayTag, GLuint>(Handle<HVertexArrayTag>& handle, GLuint value) {
		handle.m_Handle = (uintptr_t)value;
	}


}

namespace qk::gfx::rhi {




	HVertexArray VertexArray_ctor() {
		HVertexArray handle;
		GLuint native = 0;

		glGenVertexArrays(1, &native);

		set<HVertexArrayTag, GLuint>(handle, native);

		return handle;

	}

	void VertexArray_dtor(HVertexArray& hdl) {
		GLuint native = get<HVertexArrayTag, GLuint>(hdl);
		glDeleteVertexArrays(1, &native);
		hdl = nullptr;
	}

	void VertexArray_bind(const HVertexArray& hdl) {
		glBindVertexArray(get<HVertexArrayTag, GLuint>(hdl));
	}

	void VertexArray_unbind(const HVertexArray& hdl) {
		glBindVertexArray(0);
	}

	HVertexArrayRef VertexArray_get_current_bound() noexcept {
		GLint h = 0;
		glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &h);
		GLuint native = static_cast<GLuint>(h);

		HVertexArray handle;
		set<HVertexArrayTag, GLuint>(handle, native);

		return handle.copy();

	}

}