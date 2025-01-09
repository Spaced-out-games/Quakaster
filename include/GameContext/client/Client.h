#pragma once
#include <include/thirdparty/entt.hpp>
#include <include/GameContext/Scene.h>
#include <include/GameContext/UI/UIContext.h>

struct Client
{
	//entt::resource_cache<res_mesh> meshes;
	//entt::resource_cache<res_texture> meshes;
	//entt::resource_cache<res_shader> meshes;
	Scene cl_scene;
	UIContext cl_UI_context;
};