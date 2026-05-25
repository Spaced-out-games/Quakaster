#include "ConvarService.h"



namespace qk::service {
	bool ConvarService::has(const str& id) {
		ScopeTimer timer(uptime_ptr());
		return m_Registry.has(id);
	}
	bool ConvarService::set(const str& id, convar_t new_value) {
		ScopeTimer timer(uptime_ptr());
		return m_Registry.set(id, new_value);
	}

}