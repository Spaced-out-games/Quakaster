#pragma once
#include <variant>
#include <utility>

namespace qk {

	template <class T, class E>
	class Result {
		std::variant<T, E> m_Data;

	public:
		Result(const T& value) : m_Data(value) {}
		Result(T&& value) : m_Data(std::move(value)) {}

		Result(const E& error) : m_Data(error) {}
		Result(E&& error) : m_Data(std::move(error)) {}

		bool has_value() const {
			return std::holds_alternative<T>(m_Data);
		}

		bool has_error() const {
			return std::holds_alternative<E>(m_Data);
		}

		T& value() {
			return std::get<T>(m_Data);
		}

		const T& value() const {
			return std::get<T>(m_Data);
		}

		E& error() {
			return std::get<E>(m_Data);
		}

		const E& error() const {
			return std::get<E>(m_Data);
		}
	};

	struct void_t {};
}