#pragma once
#include <core.h>

namespace qk {

	template <class From_t, class To_t, class VariantTag_t, u32 version = 1>
	bool write(const From_t&, To_t&);

}