/// **************************************** QUAKASTER ENGINE **************************************** 
/// components/Result.h
/// Purpose: in-house alias to std::expected
/// **************************************************************************************************
#pragma once
#include <variant>
#include <utility>
#include <expected>

namespace qk {

	template <class result_t, class err_t>
	using Result = std::expected<result_t, err_t>;

	template <class err_t>
	using Error = std::unexpected<err_t>;


}