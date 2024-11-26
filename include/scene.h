#pragma once
#include "entt.hpp"

class Entity;

class Scene: public entt::registry {
	//entt::registry registry;
	Entity create_entity();
public:
	Scene();
};


#include "entity.h"

Entity Scene::create_entity()
{
	Entity ent(create());
	return ent;
	
}

Scene::Scene()
{
	Entity::scene = this;
}



//using Scene = entt::registry;
