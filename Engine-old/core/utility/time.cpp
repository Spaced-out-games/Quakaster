/// **************************************** QUAKASTER ENGINE **************************************** 
/// core/utility/time.cpp
/// Purpose: Implements time utility functions.
/// **************************************************************************************************
#include "time.h"
#include <chrono>

namespace qk::util {
	uint64_t now() {
		return std::chrono::duration_cast<std::chrono::nanoseconds>(
			std::chrono::high_resolution_clock::now().time_since_epoch())
			.count();
	}
}