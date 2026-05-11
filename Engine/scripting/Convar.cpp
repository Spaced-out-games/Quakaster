/// **************************************** QUAKASTER ENGINE **************************************** 
/// console/Convar.cpp
/// Purpose: Defines Convar implementatino
/// ************************************************************************************************** 

#include "Convar.h"


namespace qk::scripting {
	Convar::Convar(convar_t value, const std::string& description, convar_validate_pfn_t validator) :
		m_Value(value), m_Validator(validator), m_Description(description) {

	}

	const std::string& Convar::description() { return m_Description; }

	bool Convar::set(convar_t new_value) {

		if (m_Validator && !m_Validator(new_value)) return false;

		m_Value = new_value;
		return true;

	}

	const convar_t& Convar::value() {
		return m_Value;
	}



}