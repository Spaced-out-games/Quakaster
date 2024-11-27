#pragma once
#include <include/entt.hpp>
#include <include/think.h>

class Entity;

class Scene : public entt::registry
{
public:
	std::vector<thinkScript> scripts;
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
	void addThink(thinkScript& script) {
		scripts.push_back(std::move(script));
	}

};

#include "entity.h"

