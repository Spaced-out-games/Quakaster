/// **************************************** QUAKASTER ENGINE **************************************** 
/// utility/time.h
/// Purpose: Provides some basic time-related utilities.
/// **************************************************************************************************

#pragma once
#include <stdint.h>
#include "core.h"

namespace qk::utility {

	// Gets the time (in nanoseconds) since epoch
	QK_API uint64_t now();
}