#pragma once
#include <entt/entity/entity.hpp>
#include <entt/entity/registry.hpp>
#include "../vendor/emhash/hash_table5.hpp"
#include <variant>
#include <vector>
#include <core.h>
#include <media/json.h>
#include <fnv1a.h>
#include <KeyValues.h>
using qk::media::JSONArrayClassification;
using qk::media::flatten_json;
using qk::media::classify;
using qk::media::json;
using qk::string_hash;

namespace qk {





	using EntityComponentLoader_pfn_t = void (*)(entt::registry& registry, entt::entity target, const KeyValues& arguments);
	
	struct ComponentLoader {
		// name of this component
		str_view name;

		// which keys this component loader uses
		std::vector<str> dependencies;

		EntityComponentLoader_pfn_t			   loader = nullptr;

	};


}