#include "ConvarService.h"



namespace qk::service {
	bool ConvarService::has(const str& id) {
		return m_Registry.has(id);
	}
	bool ConvarService::set(const str& id, convar_t new_value) {
		return m_Registry.set(id, new_value);
	}

}