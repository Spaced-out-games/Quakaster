#include "HVertexLayout.h"
#include "glad/glad.h"
#include <vector>

namespace qk::gfx::rhi {

	static inline qk::HandleResourceRegistry<VertexLayoutDesc, HVertexLayout> g_VertexLayouts;


	inline GLenum ScalarType_cast(EScalarType type)
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


	HVertexLayout	VertexLayout_ctor(const VertexLayoutDesc& desc) {
		return g_VertexLayouts.insert(desc);
	}

	bool			VertexLayout_dtor(const HVertexLayout& hLayout) {
		return g_VertexLayouts.destroy(hLayout);
	}

	bool			VertexLayout_valid(const HVertexLayout& hLayout) {
		return g_VertexLayouts.valid(hLayout);
	}



	bool			VertexLayout_apply(const HVertexLayout& hLayout) {
		QK_ASSERT(g_VertexLayouts.valid(hLayout));
		if (!g_VertexLayouts.valid(hLayout)) return 0;

		VertexLayoutDesc& desc = g_VertexLayouts.get(hLayout);

		for (auto& binding : desc.bindings()) {
			for (auto& attr : binding.attributes) {

				if (EScalarType_isInteger(attr.component.comp_type) && !attr.component.comp_normalize) {
					glVertexAttribIPointer(attr.location, attr.component.comp_count, ScalarType_cast(attr.component.comp_type), binding.stride, (const void*)attr.offset);
				}
				else {
					glVertexAttribPointer(attr.location, attr.component.comp_count, ScalarType_cast(attr.component.comp_type), attr.component.comp_normalize, binding.stride, (const void*)attr.offset);
				}

				glEnableVertexAttribArray(attr.location);
				glVertexAttribDivisor(attr.location, binding.input_rate == InputRate::PerInstance ? 1 : 0);
			}
		}
		





		return 1;
	}

} // namespace qk::gfx::rhi