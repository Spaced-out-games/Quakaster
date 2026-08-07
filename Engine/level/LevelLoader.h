#pragma once
#include "../media/ComponentLoader.h"
#include "../media/json.h"
#include "fnv1a.h"
#include <span>

// temp
#include <iostream>

using qk::media::json;
using qk::media::flatten_json;
using qk::media::array_matches;
using qk::string_hash;

namespace qk {

	void deserialize_entity(entt::registry& registry, entt::entity target, const EntityArgumentMap& args, const EntityComponentLoaderMap& loader) {
		assert(target != entt::null);

		// This might not be the most efficient way of doing it but it is the most generic and easy-to-use approach.
		for (const auto& [EntityComponentID, ComponentLoader] : loader) {
			ComponentLoader(registry, target, args);
		}

	}


	// NOTE: since this function is meant to be called on *individual entities* there should never be an object inside, especially after flattening!
	


	void deserialize_scene(const json& scene, entt::registry& registry, const qk::EntityComponentLoaderMap& component_loader) {

		// make sure it's empty first
		assert(registry.storage<entt::entity>().in_use() == 0);



		if (!scene.contains("entities") || !scene["entities"].is_array())
			return;

		qk::EntityArgumentMap args;
		entt::entity entity;

		for (const auto& entity_args : scene["entities"]) {
			entity = registry.create();
			args = from_json(entity_args);
			deserialize_entity(registry, entity, args, component_loader);
		}



	}


}