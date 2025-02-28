#pragma once
#include <include/GameContext/base/eventHandler.h>
#include <include/thirdparty/entt.hpp>


namespace Quakaster::base {

	// TODO: Move functionality from dispatcher into its own wrapped type, then make it a component of Scene.
	// Since almost all of them use event_handler& already, per the custom #define in entt, it should be pretty easy to simply move it over there
	// Once that's done, you might want to double, triple check that every component properly follows conventions
	// Once THAT's done, move to defining entity types and go around, fixing that shit. 
	struct Scene
	{
		entt::registry registry;
		eventHandler event_handler;


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