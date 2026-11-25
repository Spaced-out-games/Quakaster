#include "SceneLoader.h"


namespace qk {

	std::optional<std::vector<KeyValues>> from_json(const json& j) {
		if (!j.is_array())
			return std::nullopt;


		std::vector<KeyValues> objects;

		for (const auto& entity_args : j) {

			
			auto map = KeyValues_fromJSON<str>(entity_args, '.');

			if (map) {
				objects.push_back(*map);
			} else return std::nullopt;


		}

		return objects;

	}

	void deserialize_scene(const std::span<KeyValues>& scene, entt::registry& registry, EntityLoader loader) {
		QK_ASSERT(registry.storage<entt::entity>().in_use() == 0, "Scene was not empty");

		entt::entity target = entt::null;
		for (const auto& entity_args : scene) {
			target = registry.create();
			loader.execute(registry, target, entity_args);
		}

	}
}