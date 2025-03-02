#pragma once
#include <include/thirdparty/entt.hpp>
#include <include/GameContext/base/Scene.h>
#include <include/GameContext/UI/UIContext.h>

// Minimal client
struct Client
{
	//Client(EventHandler& cl_evt_handler) : delegate(cl_evt_handler) {}
	// client-side scene
	Scene scene;

	// Temporary player ID.
	entt::entity player_ID;

	// Client-side systems
	std::vector<ISystem*> systems;

	//InputDelegate delegate;
};