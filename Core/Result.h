/// **************************************** QUAKASTER ENGINE **************************************** 
/// components/Result.h
/// Purpose: in-house alias to std::expected
/// **************************************************************************************************
#pragma once
#include <variant>
#include <utility>

namespace qk {

	// ergonomic std::variant. Should really just be replaced with std::expected.
	template <typename... Args>
	class Variant {
		std::variant<Args...> m_Value;

	public:



		template <class T>
		bool is() const {
			return std::holds_alternative<T>(m_Value);
		}

		template <class T>
		const T& get() const {
			return std::get<T>(m_Value);
		}

		template <class T>
		T& get() {
			return std::get<T>(m_Value);
		}

		template <class T>
		const T* try_get() const {
			return std::get_if<T>(&m_Value);
		}

		template <class T>
		T* try_get() {
			return std::get_if<T>(&m_Value);
		}

		template <class Visitor>
		auto visit(Visitor&& v) {
			return std::visit(std::forward<Visitor>(v), m_Value);
		}

		template <class Visitor>
		auto visit(Visitor&& v) const {
			return std::visit(std::forward<Visitor>(v), m_Value);
		}

		template <class T>
		Variant(const T& value): m_Value(value) {}

	};

	

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

	// Needs depricated
	struct void_t {};


	// Needs entirely implemented and swapped:
	//template<typename T, typename E>
	//using Result = std::expected<T, E>;

	//template<typename E>
	//using Error = std::unexpected<E>;
}