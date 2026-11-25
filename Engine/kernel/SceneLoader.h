#pragma once
#include <entt/entity/component.hpp>
#include <entt/entity/entity.hpp>
#include "ComponentLoader.h"
#include "EntityLoader.h"
#include <span>

namespace qk {

	// Loads a list of key-value pairs from a JSON file
	std::optional<std::vector<KeyValues>> from_json(const json& j);

	// Loads a list of key-value pairs into a registry, using a component loader
	void deserialize_scene(const std::span<KeyValues>& scene, entt::registry& registry, EntityLoader loader);

}