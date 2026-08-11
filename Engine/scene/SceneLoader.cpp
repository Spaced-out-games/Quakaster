#include "SceneLoader.h"


namespace qk {

	bool from_json(const json& j, std::vector<EntityArgumentMap>& scene) {
		if (!j.is_array())
			return false;

		EntityArgumentMap map;
		for (const auto& entity_args : j) {
			if (from_json(entity_args, map))
				scene.push_back(map);
			else
				return false;
		}

		return true;

	}

	void deserialize_scene(const std::span<EntityArgumentMap>& scene, entt::registry& registry, EntityLoader loader) {
		assert(registry.storage<entt::entity>().in_use() == 0);

		entt::entity target = entt::null;
		for (const auto& entity_args : scene) {
			target = registry.create();
			loader.execute(registry, target, entity_args);
		}

	}
}