/// **************************************** QUAKASTER ENGINE **************************************** 
/// console/ConvarRegistry.h
/// Purpose: Implements ConvarRegistry.
/// ************************************************************************************************** 
#include "ConvarRegistry.h"


namespace qk::scripting {
	bool ConvarRegistry::has(const std::string& id) {
		return m_Convars.contains(id);
	}

	bool ConvarRegistry::set(const std::string& id, convar_t new_value) {
		auto it = m_Convars.find(id);
		if (it == m_Convars.end()) return false;
		it->second.set(new_value);
		return true;
	}
}