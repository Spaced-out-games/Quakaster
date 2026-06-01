#include "HVertexLayout.h"
#include "glad/glad.h"
#include <vector>

namespace qk::gfx::rhi {


	struct VertexLayout_impl {
		std::vector<VertexAttributeBinding> bindings;
	};


	//static inline qk::HandleResourceRegistry<VertexLayout_createinfo, HVertexLayout> g_VertexLayouts;
	static inline rhi::ResourceCache<VertexLayout_impl, RVertexLayout> g_VertexLayouts2;



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


	RVertexLayout	VertexLayout_ctor(str_view label, const VertexLayout_createinfo& desc) {
		return g_VertexLayouts2.insert(label, VertexLayout_impl{ .bindings = desc.bindings });
	}

	bool			VertexLayout_dtor(const RVertexLayout& hLayout) {
		return g_VertexLayouts2.destroy(hLayout);
	}

	bool			VertexLayout_valid(const RVertexLayout& hLayout) {
		return g_VertexLayouts2.valid(hLayout);
	}

	bool VertexLayout_apply(const RVertexLayout& hLayout) {
		auto* pLayout = g_VertexLayouts2.try_get(hLayout);

		if (!pLayout) return 0;

		for (auto& binding : pLayout->bindings) {
			for (auto& attr : binding.attributes) {

				if (EScalarType_isInteger(attr.component.type) && !attr.component.normalize) {
					glVertexAttribIPointer(attr.location, attr.component.count, ScalarType_cast(attr.component.type), binding.stride, (const void*)attr.offset);
				}
				else {
					glVertexAttribPointer(attr.location, attr.component.count, ScalarType_cast(attr.component.type), attr.component.normalize, binding.stride, (const void*)attr.offset);
				}

				glEnableVertexAttribArray(attr.location);
				glVertexAttribDivisor(attr.location, binding.input_rate == InputRate::PerInstance ? 1 : 0);
			}
		}


		return 1;


	}


} // namespace qk::gfx::rhi