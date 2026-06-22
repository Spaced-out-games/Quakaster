/// **************************************** QUAKASTER ENGINE **************************************** 
/// platform/Quakaster.h 
/// Purpose: Manages platform initialization and shutdown
/// **************************************************************************************************
#pragma once
#include "core.h"


namespace qk {
	QK_API bool startup();
	QK_API void shutdown();
}