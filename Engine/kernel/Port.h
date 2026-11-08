#pragma once
#include <optional>

namespace qk {

	// distinct port type for signals. src creates ports; dest sets state, dest invokes src's handler, src resets each port.
	template <class T>
	struct CPort {

		std::optional<T> inbox;

	};

}