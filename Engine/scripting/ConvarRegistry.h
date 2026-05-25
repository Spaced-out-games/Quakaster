/// **************************************** QUAKASTER ENGINE **************************************** 
/// console/ConvarRegistry.h
/// Purpose: Wraps convar storage in a convenient hash map
/// ************************************************************************************************** 


// TODO: add bool has(),  maybe iterator support

#pragma once
#include "Convar.h"
#include "core.h"
#include <unordered_map>
#include <string>

namespace qk::scripting {

	class QK_API ConvarRegistry {
		std::unordered_map<str, Convar> m_Convars;

	public:
		bool has(const str& id);

		bool set(const str& id, convar_t new_value);


		template <class T>
		T* try_get(const str& id) {
			auto it = m_Convars.find(id);
			if (it == m_Convars.end()) return nullptr;
			return it->second.try_get<T>();
		}




	};

}