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

/*
	TODO: level loading overhaul:
		Introduce a ComponentLoader struct, which is just a pfn + key_hash[] + str_view holding component name (in debug)
		The EntityLoader class takes in a list of ComponentLoaders, compiles a hash map of key hash -> pfn (+str_view in debug).
		When it comes time to actually load an entity from a hash map, we give the EntityLoader the registry, the entity, and the arguments, and it will:
			-iterate through each symbol
			-hash it
			-check if a component loader needs that symbol and it's not in the used-set
				-if it does, use it, add it to the used-set
				-if it doesn't, throw out a warning
			-profit???

		Also consider better naming convention: Scene is more `correct` than `level`


*/

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