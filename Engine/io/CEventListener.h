#pragma once
#include <entt/entity/entity.hpp>
#include <entt/entity/registry.hpp>
#include <io/Event.h>

// Listens for qk Events and does somethign about it.
namespace qk {

	class CEventListener {
		using OnEventListener_pfn_t = void(*)(entt::entity target, entt::registry& registry, const Event& evt);

		OnEventListener_pfn_t m_Impl = nullptr;

		public:

			CEventListener(OnEventListener_pfn_t impl = nullptr);

			CEventListener& operator=(OnEventListener_pfn_t impl);


			void operator()(entt::entity target, entt::registry& registry, const Event& evt);




	};



}