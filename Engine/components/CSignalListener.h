#pragma once
#include <entt/entity/entity.hpp>
#include <entt/entity/registry.hpp>
#include "fnv1a.h"
#include <entt/entt.hpp>
namespace qk {

	void signal(entt::registry& reg, entt::entity src, entt::entity dest, string_hash sig);

	struct CSignalListener {

		using SignalListener_pfn_t = void (*)(entt::registry& reg, entt::entity src, entt::entity dest, string_hash sig);

		SignalListener_pfn_t SignalListener_pfn = nullptr;

		void operator()(entt::registry& reg, entt::entity src, entt::entity dest, string_hash sig);


		CSignalListener(SignalListener_pfn_t listener = nullptr);

	};

}