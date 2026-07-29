#pragma once
#include "../media/ComponentLoader.h"
#include "../media/json.h"
#include <span>
using qk::media::json;

namespace qk {

	template <class ...Ts>
	void AddComponents(entt::registry& registry, entt::entity target, const IArgumentMap<Ts...>& args, const IComponentLoaderMap<Ts...>& loader) {
		assert(target != entt::null);

		// This might not be the most efficient way of doing it but it is the most generic and easy-to-use approach.
		for (const auto& [ComponentID, ComponentLoader] : loader) {
			ComponentLoader(registry, target, args);
		}

	}

	template <class... Ts>
	std::vector<entt::entity> LoadLevel(
		entt::registry& registry,
		std::span<const IArgumentMap<Ts...>> argset,
		const IComponentLoaderMap<Ts...>& loader)
	{
		assert(registry.storage<entt::entity>().in_use() == 0);
		std::vector<entt::entity> entities;
		entities.reserve(argset.size());

		for (const auto& map : argset) {
			entt::entity target = registry.create();
			AddEntity(registry, target, map, loader);
			entities.emplace_back(target);
		}

		return entities;

	}

}