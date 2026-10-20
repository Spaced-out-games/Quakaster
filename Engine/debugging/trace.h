#pragma once
#include <source_location>
#include <format>
#include <core.h>

namespace qk {


	inline str trace(str_view what, bool debugbreak = false, std::source_location location = std::source_location::current()) {
		if (debugbreak) __debugbreak();
		return std::format("file `{}` @[{}:{}]({}): {}", location.file_name(), location.line(), location.column(),location.function_name(), what);
	}

}