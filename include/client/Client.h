#pragma once
#include <include/thirdparty/entt.hpp>
#include <include/base/Scene.h>
#include <include/UI/UIContext.h>
#include <include/base/SystemManager.h>
#include <include/server/interpreter/Convar.h>

// Minimal client
struct Client : SystemManager
{
	Client() {
		add_convar("fov_desired", 70.0f);

	}

	friend class Application;

	inline void init_system(int index) {
		SystemManager::init_system(index, scene);
	}

	inline void init_all() {
		for (ISystem* system : systems) {
			system->init(scene);
		}
	}
	inline void tick_all() {
		for (ISystem* system : systems) {
			system->tick(scene);
		}
	}


	inline void tick_system(int index) {
		SystemManager::tick_system(index, scene);
	}

	inline void destroy_system(int index) {
		SystemManager::destroy_system(index, scene);
	}
	Scene scene;


	protected:
		// client-side scene

		// Temporary player ID.
		entt::entity player_ID;

};