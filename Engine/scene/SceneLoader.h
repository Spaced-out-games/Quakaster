#pragma once
#include <entt/entity/component.hpp>
#include <entt/entity/entity.hpp>
#include "ComponentLoader.h"
#include "EntityLoader.h"
#include <span>

namespace qk {

	using Scene = std::span<EntityArgumentMap>;


	void deserialize_scene(const Scene& scene, entt::registry& registry, EntityLoader loader) {
		assert(registry.storage<entt::entity>().in_use() == 0);

		entt::entity target = entt::null;
		for (const auto& entity_args : scene) {
			target = registry.create();
			loader.execute(registry, target, entity_args);
		}

	}

}