#pragma once
#include <optional>

namespace qk {

	// distinct port type for signals
	template <class T>
	struct CPort {

		std::optional<T> inbox;

	};

}