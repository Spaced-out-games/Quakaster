#pragma once
#include <entt/entity/entity.hpp>
#include <entt/entity/registry.hpp>
#include "../vendor/emhash/hash_table5.hpp"
#include <variant>
#include <vector>
#include <core.h>
#include <media/json.h>
#include <fnv1a.h>

using qk::media::JSONArrayClassification;
using qk::media::flatten_json;
using qk::media::classify;
using qk::media::json;
using qk::string_hash;

namespace qk {

	using EntityComponentArgument = std::variant<bool, i32, u32, f32, str, std::vector<bool>, std::vector<i32>, std::vector<u32>, std::vector<f32>, std::vector<str>>;

	bool from_json(const json& j, EntityComponentArgument& arg);

	using EntityComponentArgumentID = u64;

	using EntityArgumentMap = emhash5::HashMap<EntityComponentArgumentID, EntityComponentArgument>;

	bool from_json(const json& j, EntityArgumentMap& out);

	using EntityComponentLoader_pfn_t = void (*)(entt::registry& registry, entt::entity target, const EntityArgumentMap& arguments);
	
	struct ComponentLoader {
		// name of this component
		str_view name;

		// which key hashes this component loader uses
		std::vector<EntityComponentArgumentID> dependencies;

		EntityComponentLoader_pfn_t			   loader = nullptr;

	};


}