#include "HVertexArray.h"
#include "glad\glad.h"
#include <Handle.h>
#include "Scalar.h"





namespace qk::gfx::rhi {

	struct VertexArray_impl {
		GLuint hVertexArray = 0;
	};


	//static inline qk::HandleResourceRegistry<GLuint, HVertexArray> g_VertexArrays;

	static inline rhi::ResourceCache<VertexArray_impl, HVertexArray> g_VertexArrays;


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

	HVertexArray VertexArray_ctor(str_view label) {
		GLuint hVertexArray = 0;
		glGenVertexArrays(1, &hVertexArray);


		return HVertexArray(g_VertexArrays.insert(label, VertexArray_impl{hVertexArray}));

	}

	void VertexArray_dtor(HVertexArray& hVertexArray) {
		if (!g_VertexArrays.valid(hVertexArray)) return;

		auto* pVertexArray = g_VertexArrays.try_get(hVertexArray);

		if (!pVertexArray) return;
		glDeleteVertexArrays(1, &pVertexArray->hVertexArray);
		g_VertexArrays.destroy(hVertexArray);
		
	}

	bool VertexArray_bind(const HVertexArray& hVertexArray) {
		auto* pVertexArray = g_VertexArrays.try_get(hVertexArray);
		if (!pVertexArray) return 0;

		glBindVertexArray(pVertexArray->hVertexArray);

		return 1;
	}

	bool VertexArray_valid(const HVertexArray& hVertexArray) {
		return g_VertexArrays.valid(hVertexArray);
	}



}