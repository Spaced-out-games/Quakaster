#pragma once
#include <entt/entity/component.hpp>
#include <entt/entity/entity.hpp>
#include "ComponentLoader.h"
#include "EntityLoader.h"
#include <span>

namespace qk {


	bool from_json(const json& j, std::vector<EntityArgumentMap>& scene);

	void deserialize_scene(const std::span<EntityArgumentMap>& scene, entt::registry& registry, EntityLoader loader);

}