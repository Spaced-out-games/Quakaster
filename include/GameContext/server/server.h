#pragma once
#include "entt.hpp"
#include "ConsoleInterpreter.h"
#include "scene.h"


struct Server
{
	//entt::resource_cache<res_mesh> meshes;
	//entt::resource_cache<res_texture> meshes;
	//entt::resource_cache<res_shader> meshes;
	ConsoleInterpreter sv_console_interpreter;
	Scene sv_scene;

};