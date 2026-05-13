#include "HVertexArray.h"
#include "HVertexBufferLayout.h"
#include "glad\glad.h"

using qk::gfx::rhi::HVertexArray;
using qk::gfx::rhi::HVertexArrayTag;
using qk::gfx::rhi::HVertexBufferLayout;
using qk::gfx::rhi::HVertexBufferLayoutTag;
using qk::gfx::rhi::VertexBufferLayoutDesc;




namespace qk {
	template<>
	struct handle_traits<HVertexArrayTag> {
		using value_type = GLuint;
	};

}

namespace qk {
	template<>
	struct handle_traits<HVertexBufferLayoutTag> {
		using value_type = VertexBufferLayoutDesc*;
	};

}




namespace qk::gfx::rhi {

	inline GLenum ScalarType_cast(ScalarType type)
	{
		switch (type)
		{

			case ScalarType::i8:
				return GL_BYTE;

			case ScalarType::i16:
				return GL_SHORT;

			case ScalarType::i32:
				return GL_INT;

			case ScalarType::u8:
				return GL_UNSIGNED_BYTE;

			case ScalarType::u16:
				return GL_UNSIGNED_SHORT;

			case ScalarType::u32:
				return GL_UNSIGNED_INT;

			case ScalarType::f32:
				return GL_FLOAT;

			case ScalarType::f64:
				return GL_DOUBLE;

			default:
				return GL_NONE;

		}
	}

	inline bool ScalarType_isInteger(ScalarType type)
	{
		switch (type)
		{
		case ScalarType::i8:
		case ScalarType::i16:
		case ScalarType::i32:
		case ScalarType::u8:
		case ScalarType::u16:
		case ScalarType::u32:
			return true;

		default:
			return false;
		}
	}



	HVertexArray VertexArray_ctor() {
		HVertexArray handle;
		GLuint native = 0;

		glGenVertexArrays(1, &native);

		set(handle, native);

		return handle;

	}

	void VertexArray_dtor(HVertexArray& hdl) {
		GLuint native = get(hdl);
		glDeleteVertexArrays(1, &native);
		hdl = nullptr;
	}

	void VertexArray_bind(const HVertexArray& hdl) {
		glBindVertexArray(get(hdl));
	}

	void VertexArray_unbind() {
		glBindVertexArray(0);
	}
	/*
	HVertexArrayRef VertexArray_get_current_bound() noexcept {
		GLint h = 0;
		glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &h);
		GLuint native = static_cast<GLuint>(h);

		HVertexArray handle;
		set<HVertexArrayTag, GLuint>(handle, native);

		return handle.copy();

	}*/

	void VertexArray_applyLayout(
		const HVertexArray& hVertexArray,
		const HVertexBuffer& hVertexBuffer,
		const HVertexBufferLayout& hLayout
	) {
		VertexArray_bind(hVertexArray);
		VertexBuffer_bind(hVertexBuffer);


		VertexBufferLayoutDesc* layout = get(hLayout);

		assert(layout);

		if (!layout) return;

		for (const auto& elem : layout->attributes())
		{
			glEnableVertexAttribArray(elem.location);

			if (ScalarType_isInteger(elem.attr.type) && !elem.attr.normalize)
			{
				glVertexAttribIPointer(
					elem.location,
					elem.attr.count,
					ScalarType_cast(elem.attr.type),
					layout->stride(elem.binding),
					(void*)(uintptr_t)elem.offset
				);
			}
			else
			{
				glVertexAttribPointer(
					elem.location,
					elem.attr.count,
					ScalarType_cast(elem.attr.type),
					elem.attr.normalize,
					layout->stride(elem.binding),
					(void*)(uintptr_t)elem.offset
				);
			}
		}

	}




}