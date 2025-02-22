#pragma once
#include <include/thirdparty/entt.hpp>
#include <include/GameContext/base/Scene.h>
#include <include/GameContext/UI/UIContext.h>


struct Client
{
	//entt::resource_cache<res_mesh> meshes;
	//entt::resource_cache<res_texture> meshes;
	//entt::resource_cache<res_shader> meshes;
	Quakaster::base::Scene cl_scene;
	UIContext cl_UI_context;
};