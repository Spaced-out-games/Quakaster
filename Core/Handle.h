/// **************************************** QUAKASTER ENGINE **************************************** 
/// Core/Result.h
/// Purpose: Defines an opaque Resource type that smuggles Resources past encapsulation borders.
///          Also defines an opaque reference to these Resources
/// **************************************************************************************************
#pragma once
#include <cstdint>
#include <utility>

namespace qk {

	struct Handle {
		uint32_t index;
		uint32_t generation;
	};

} // namespace qk