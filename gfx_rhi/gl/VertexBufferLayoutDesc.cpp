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
	// Push attribute
	// ------------------------------------------------------------
	void VertexBufferLayoutDesc::push(
		ScalarType type,
		uint16_t count,
		bool normalize,
		InputRate rate,
		ScalarMultiplicity multiplicity,
		uint32_t location
	)
	{
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
		// compute offset (packing attributes sequentially)
		// ----------------------------------------
		uint32_t offset = 0;

		for (const auto& existing : m_Attributes)
		{
			uint32_t size =
				existing.attr.scalar.count *
				ScalarType_size(existing.attr.scalar.type);

			uint32_t end = existing.offset + size;

			if (end > offset)
				offset = end;
		}

		// ----------------------------------------
		// create attribute
		// ----------------------------------------
		VertexAttributeBinding attr;
		attr.attr = VertexAttribute{ ScalarDesc{multiplicity, type, count}, normalize };
		attr.location = location;
		attr.offset = offset;
		attr.rate = rate;

		m_Attributes.push_back(attr);

		m_LastLocation = location;

		// ----------------------------------------
		// update stride (IMPORTANT FIX)
		// ----------------------------------------
		uint32_t attrSize = count * ScalarType_size(type);

		m_Stride = offset + attrSize;
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
	/*
	const std::vector<VertexBufferBinding>&
		VertexBufferLayoutDesc::bindings() const
	{
		return m_Bindings;
	}
	*/
	// ------------------------------------------------------------
	// stride query
	// ------------------------------------------------------------
	uint32_t VertexBufferLayoutDesc::stride(uint32_t) const
	{
		return m_Stride;
	}

}