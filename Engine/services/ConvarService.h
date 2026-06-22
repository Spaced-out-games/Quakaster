/// **************************************** QUAKASTER ENGINE **************************************** 
/// services/ConvarService.h
/// Purpose: Provides a service to add, remove, and modify console variables by name.
/// **************************************************************************************************

#pragma once
#include "../scripting/ConvarRegistry.h"
#include "../utility/ScopeTimer.h"
#include <string>
#include "IService.h"
#include "core.h"

using namespace qk::scripting;
using namespace qk::utility;


namespace qk::service {

	// Maintains a registry of console variables and provides an interface to work with them
	class QK_API ConvarService : IService {

		
		ConvarRegistry m_Registry;

	public:
		bool has(const str& id);
		bool set(const str& id, convar_t new_value);
		template <class T>
		T* try_get(const str& id) {
			ScopeTimer timer(uptime_ptr());
			return m_Registry.try_get<T>(id);
		}
	};

}