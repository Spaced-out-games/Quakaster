#include "Scalar.h"

namespace qk::gfx::rhi {
	size_t ScalarType_size(ScalarType type)
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
}