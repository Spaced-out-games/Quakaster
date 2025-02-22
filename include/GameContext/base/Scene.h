#pragma once
//#include "entt.hpp"
#include <include/thirdparty/entt.hpp>

namespace Quakaster::base {


	struct Scene
	{
		entt::registry registry;

		template <typename T, typename ...Args>
		inline T& add_component(entt::entity entity, Args&&... args) {
			return registry.emplace<T>(entity, std::forward<Args>(args)...);
		}

		template <typename ent_t = IEntity, typename ...Args>
		inline ent_t& create(Args&&... args) {
			// Create a new entity in the registry
			entt::entity entity = registry.create();

			// Create the entity instance and return a reference to it
			return registry.emplace<ent_t>(entity, entity, *this, std::forward<Args>(args)...);
		}
	};


}