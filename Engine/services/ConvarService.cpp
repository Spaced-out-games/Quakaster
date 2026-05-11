#include "ConvarService.h"



namespace qk::service {
	bool ConvarService::has(const std::string& id) {
		ScopeTimer timer(uptime_ptr());
		return m_Registry.has(id);
	}
	bool ConvarService::set(const std::string& id, convar_t new_value) {
		ScopeTimer timer(uptime_ptr());
		return m_Registry.set(id, new_value);
	}

}