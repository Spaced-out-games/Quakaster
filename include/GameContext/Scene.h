#pragma once
//#include "entt.hpp"
#include <include/thirdparty/entt.hpp>

struct Scene
{
	entt::registry registry;

	entt::handle make_entity()
	{
		return entt::handle{ registry, registry.create() };
	}
};