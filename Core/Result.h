#pragma once
#include <expected>



namespace qk {

	template <class T, class E>
	using Result = std::expected<T, E>;

}