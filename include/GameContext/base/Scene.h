#pragma once
#include <include/GameContext/base/eventHandler.h>
#include <include/thirdparty/entt.hpp>
#include <include/GameContext/base/Component.h>


namespace Quakaster::base {

	// TODO: Move functionality from dispatcher into its own wrapped type, then make it a component of Scene.
	// Since almost all of them use event_handler& already, per the custom #define in entt, it should be pretty easy to simply move it over there
	// Once that's done, you might want to double, triple check that every component properly follows conventions
	// Once THAT's done, move to defining entity types and go around, fixing that shit. 
	struct Scene
	{
		entt::registry registry;
		EventHandler event_handler;


		template <typename T, typename ...Args>
		inline T& add_component(entt::entity entity, Args&&... args) {
			//static_assert(std::is_base_of_v<Quakaster::base::Component, T>, "T must be a component type.");
			return registry.emplace<T>(entity, std::forward<Args>(args)...);
		}




		template <typename T>
		inline T& get_component(entt::entity entity) {
			return registry.get<T>(entity);
		}

		template <typename T>
		inline T* try_get_component(entt::entity entity) {
			return registry.try_get<T>(entity);
		}

		template <typename T>
		inline bool has(entt::entity entity) {
			return registry.all_of<T>(entity);
		}

		template <typename ...Ts>
		inline bool has_all_of(entt::entity entity) {
			return registry.all_of<Ts...>(entity);
		}
		template <typename ...Ts>
		inline bool has_one_of(entt::entity entity) {
			return registry.any_of<Ts...>(entity);
		}
		template <typename T>
		inline void remove_component(entt::entity entity) {
			registry.remove<T>(entity);
		}

		entt::registry& get_registry() { return registry; }

		template <typename... Ts>
		inline auto view() {
			return registry.view<Ts...>();
		}

	};


}

using Scene = Quakaster::base::Scene;