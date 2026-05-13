#include "VertexBufferLayoutDesc.h"
#include <cassert>

namespace qk::gfx::rhi {

	size_t QK_API ScalarType_size(ScalarType type)
	{
		switch (type)
		{
		case ScalarType::unknown: return 0;

		case ScalarType::i8:  return 1;
		case ScalarType::i16: return 2;
		case ScalarType::i32: return 4;
		case ScalarType::i64: return 8;

		case ScalarType::u8:  return 1;
		case ScalarType::u16: return 2;
		case ScalarType::u32: return 4;
		case ScalarType::u64: return 8;

		case ScalarType::f32: return 4;
		case ScalarType::f64: return 8;

		case ScalarType::ptr: return sizeof(void*);

		default:
			return 0;
		}
	}

	// ------------------------------------------------------------
	// Add a binding stream
	// ------------------------------------------------------------
	void VertexBufferLayoutDesc::add_binding(
		uint32_t binding,
		uint32_t stride,
		InputRate rate
	)
	{
		m_Bindings.push_back(VertexBufferBinding{
			.stride = stride,
			.binding = binding,
			.rate = rate
			});

		m_LastBinding = binding;
	}

	// ------------------------------------------------------------
	// Push attribute
	// ------------------------------------------------------------
	void VertexBufferLayoutDesc::push(
		const VertexAttribute& attr,
		uint32_t location,
		uint32_t binding
	)
	{
		// ----------------------------------------
		// auto binding
		// ----------------------------------------
		if (binding == AUTO)
		{
			assert(m_LastBinding != AUTO);
			binding = m_LastBinding;
		}

		// ----------------------------------------
		// auto location
		// ----------------------------------------
		if (location == AUTO)
		{
			if (m_LastLocation == AUTO)
				location = 0;
			else
				location = m_LastLocation + 1;
		}

		// ----------------------------------------
		// compute offset
		// ----------------------------------------
		uint32_t offset = 0;

		for (const auto& existing : m_Attributes)
		{
			if (existing.binding != binding)
				continue;

			uint32_t end =
				existing.offset +
				(existing.attr.count * ScalarType_size(existing.attr.type));

			if (end > offset)
				offset = end;
		}

		// ----------------------------------------
		// push attribute
		// ----------------------------------------
		m_Attributes.push_back(VertexAttributeBinding{
			.attr = attr,
			.location = location,
			.offset = offset,
			.binding = binding,
			});

		m_LastLocation = location;
		m_LastBinding = binding;

		// ----------------------------------------
		// auto stride update
		// ----------------------------------------
		for (auto& b : m_Bindings)
		{
			if (b.binding != binding)
				continue;

			// if stride was AUTO, infer it
			if (b.stride == AUTO)
			{
				b.stride =
					offset +
					(attr.count * ScalarType_size(attr.type));
			}
		}
	}

	// ------------------------------------------------------------
	// attributes
	// ------------------------------------------------------------
	const std::vector<VertexAttributeBinding>&
		VertexBufferLayoutDesc::attributes() const
	{
		return m_Attributes;
	}

	// ------------------------------------------------------------
	// bindings
	// ------------------------------------------------------------
	const std::vector<VertexBufferBinding>&
		VertexBufferLayoutDesc::bindings() const
	{
		return m_Bindings;
	}

	// ------------------------------------------------------------
	// stride query
	// ------------------------------------------------------------
	uint32_t VertexBufferLayoutDesc::stride(uint32_t binding) const
	{
		for (const auto& b : m_Bindings)
		{
			if (b.binding == binding)
				return b.stride;
		}

		return 0;
	}

}