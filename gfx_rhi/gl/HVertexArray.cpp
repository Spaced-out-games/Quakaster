#include "HVertexArray.h"
#include "glad\glad.h"
#include <Handle.h>
#include "Scalar.h"





namespace qk::gfx::rhi {


	static inline qk::HandleResourceRegistry<GLuint, HVertexArray> g_VertexArrays;



	inline GLenum EScalarType_cast(EScalarType type)
	{
		switch (type)
		{

			case EScalarType::i8:
				return GL_BYTE;

			case EScalarType::i16:
				return GL_SHORT;

			case EScalarType::i32:
				return GL_INT;

			case EScalarType::u8:
				return GL_UNSIGNED_BYTE;

			case EScalarType::u16:
				return GL_UNSIGNED_SHORT;

			case EScalarType::u32:
				return GL_UNSIGNED_INT;

			case EScalarType::f32:
				return GL_FLOAT;

			case EScalarType::f64:
				return GL_DOUBLE;

			default:
				return GL_NONE;

		}
	}

	HVertexArray VertexArray_ctor() {
		GLuint GL_vertexArray = 0;
		glGenVertexArrays(1, &GL_vertexArray);


		return Handle_cast<HVertexArray, Handle>(g_VertexArrays.insert(GL_vertexArray));

	}

	void VertexArray_dtor(HVertexArray& hVertexArray) {
		if (!g_VertexArrays.valid(hVertexArray)) return;

		GLuint target = g_VertexArrays.get(hVertexArray);
		g_VertexArrays.destroy(hVertexArray);
		glGenVertexArrays(1, &target);
		
	}

	void VertexArray_bind(const HVertexArray& hVertexArray) {
		
		if (!g_VertexArrays.valid(hVertexArray)) return;
		GLuint target = g_VertexArrays.get(hVertexArray);

		glBindVertexArray(target);

	}

	bool VertexArray_valid(const HVertexArray& hVertexArray) {
		return g_VertexArrays.valid(hVertexArray);
	}

	/*
	HVertexArray VertexArray_ctor() {
		HVertexArray handle;
		GLuint native = 0;

		glGenVertexArrays(1, &native);

		set(handle, native);

		return handle;

	}

	void VertexArray_dtor(HVertexArray& hVertexArray) {
		QK_ASSERT(hVertexArray.valid());
		GLuint gl_vao = get(hVertexArray);
		glDeleteVertexArrays(1, &gl_vao);
		hVertexArray = nullptr;
	}

	void VertexArray_bind(const HVertexArray& hVertexArray) {
		QK_ASSERT(hVertexArray.valid());
		glBindVertexArray(get(hVertexArray));
	}

	void VertexArray_unbind() {
		glBindVertexArray(0);
	}
	*/
	/*
	HVertexArrayRef VertexArray_get_current_bound() noexcept {
		GLint h = 0;
		glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &h);
		GLuint native = static_cast<GLuint>(h);

		HVertexArray handle;
		set<HVertexArrayTag, GLuint>(handle, native);

		return handle.copy();

	}
	*/
	/*
	void VertexArray_applyLayout(
		const HVertexArray& hVertexArray,
		const HVertexBuffer& hVertexBuffer,
		const HVertexBufferLayout& hLayout
	) {
		QK_ASSERT(hVertexBuffer.valid());
		QK_ASSERT(hVertexArray.valid());
		QK_ASSERT(hLayout.valid());


		VertexArray_bind(hVertexArray);
		VertexBuffer_bind(hVertexBuffer);


		VertexBufferLayoutDesc* layout = get(hLayout);

		if (!layout) return;

		for (const auto& elem : layout->attributes())
		{

			std::cout << "location: " << elem.location;
			std::cout << "\nbinding: " << elem.binding;
			std::cout << "\noffset: " <<elem.offset << "\n\n";

			glEnableVertexAttribArray(elem.location);

			if (EScalarType_isInteger(elem.attr.scalar.type) && !elem.attr.normalize)
			{
				glVertexAttribIPointer(
					elem.location,
					elem.attr.scalar.count,
					EScalarType_cast(elem.attr.scalar.type),
					layout->stride(elem.binding),
					(void*)(uintptr_t)elem.offset
				);
			}
			else
			{
				glVertexAttribPointer(
					elem.location,
					elem.attr.scalar.count,
					EScalarType_cast(elem.attr.scalar.type),
					elem.attr.normalize,
					layout->stride(elem.binding),
					(void*)(uintptr_t)elem.offset
				);
			}

			glVertexAttribDivisor(
				elem.location,
				elem.rate == InputRate::Instance ? 1 : 0
			);

		}

	}
	*/



}