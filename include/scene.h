#pragma once
#include <include/entt.hpp>
#include <include/think.h>




class Entity;
class Application;

/// <summary>
/// Scene containing all the objects in the game world. Might be a good idea to allow the Scene to have access to the GameContext (NOT IMPLEMENTED)
/// for access to things like rendering, GUI shit, window state information... 
/// </summary>
class Scene : public entt::registry
{
public:
	entt::dispatcher dispatcher;
	Application* owner;

	// Functions to be called every tick iteration (not necessarily every frame
	std::vector<thinkScript> scripts;

	// Attempts to call all think scripts, but only succeeds if it's actually scheduled to do so
	void tick()
	{
		for (thinkScript& script : scripts)
		{
			bool attempt_tick = script.shouldTick();
			if (attempt_tick)
			{
				script(*this);
				script.advanceTick();
			}
		}
	}

	// Adds a think script to the game world
	void addThink(func_ptr_t<void, Scene&> func, int tps = 60) {
		scripts.emplace_back(func, tps);
	}

};

