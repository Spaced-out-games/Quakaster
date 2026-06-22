/// **************************************** QUAKASTER ENGINE **************************************** 
/// scripting/Convar.h
/// Purpose: Defines Console variables
/// ************************************************************************************************** 


#pragma once
#include <variant>
#include "core.h"
#include <string>

namespace qk::scripting {

	// convar variant type
	using convar_t = std::variant<int, str, float, bool>;

	// function that validates a convar
	using convar_validate_pfn_t = bool(*)(const convar_t&);

	/// <summary>
	/// Console variable type. Stores a variant, a description, and a validation callback
	/// </summary>
	class QK_API Convar {

		convar_t m_Value;
		convar_validate_pfn_t m_Validator = nullptr;
		str m_Description;

	public:

		Convar(convar_t value = 0, const str& description = "No Description Provided", convar_validate_pfn_t validator = nullptr);

		// Gets the description of the convar
		const str& description();

		/// <summary>
		/// Gets a pointer to the underlying type, returns nullptr on failure
		/// </summary>
		/// <typeparam name="T">the type</typeparam>
		/// <returns>a pointer to the underlying item; nullptr on failure</returns>
		template <class T>
		T* try_get() {
			return std::get_if<T>(&m_Value);
		}

		/// <summary>
		/// Gets a pointer to the underlying type, returns nullptr on failure
		/// </summary>
		/// <typeparam name="T">the type</typeparam>
		/// <returns>a pointer to the underlying item; nullptr on failure</returns>
		template <class T>
		const T* try_get() const {
			return std::get_if<T>(&m_Value);
		}

		// type-safe setting
		bool set(convar_t new_value);

		// Gets the value of the convar.
		const convar_t& value();



	};
}