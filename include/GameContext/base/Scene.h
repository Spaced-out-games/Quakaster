#pragma once
//#include "entt.hpp"
#include <include/thirdparty/entt.hpp>
#include <include/GameContext/base/Component.h>

namespace Quakaster::base {


	struct Scene
	{
		entt::registry registry;

		template <typename T, typename ...Args>
		inline T& add_component(entt::entity entity, Args&&... args) {
			static_assert(std::is_base_of_v<Quakaster::base::Component, T>, "T must be a component type.");
			return registry.emplace<T>(entity, std::forward<Args>(args)...);
		}

		template <typename ent_t = IEntity, typename ...Args>
		ent_t create(Args&&... args) {
			// Create a new entity in the registry
			entt::entity entity = registry.create();

			return {entity, *this, std::forward<Args>(args)... };
		}

		template <typename T>
		inline T& get_component(entt::entity entity) {
			return registry.get<T>(entity);
		}

		template <typename T>
		inline T* try_get_component(entt::entity entity) {
			return registry.try_get<T>(entity);
		}

		entt::registry& get_registry() { return registry; }

	};


}