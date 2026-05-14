#pragma once
#include <variant>
#include <utility>

namespace qk {

	template <class T, class E>
	class Result {
		std::variant<T, E> m_Result;

	public:
		Result(const T& value) : m_Result(value) {}
		Result(T&& value) : m_Result(std::move(value)) {}

		Result(const E& error) : m_Result(error) {}
		Result(E&& error) : m_Result(std::move(error)) {}

		bool has_value() const {
			return std::holds_alternative<T>(m_Result);
		}

		bool has_error() const {
			return std::holds_alternative<E>(m_Result);
		}

		T& value() {
			return std::get<T>(m_Result);
		}

		const T& value() const {
			return std::get<T>(m_Result);
		}

		E& error() {
			return std::get<E>(m_Result);
		}

		const E& error() const {
			return std::get<E>(m_Result);
		}

		explicit operator bool() const {
			return has_value();
		}

	};

	struct void_t {};
}