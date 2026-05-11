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

	class QK_API ConvarService : IService {

		
		ConvarRegistry m_Registry;

	public:
		bool has(const std::string& id);
		bool set(const std::string& id, convar_t new_value);
		template <class T>
		T* try_get(const std::string& id) {
			ScopeTimer timer(uptime_ptr());
			return m_Registry.try_get<T>(id);
		}
	};

}