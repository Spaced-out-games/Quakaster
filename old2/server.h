#pragma once
#include "entt.hpp"
#include "consoleInterpreter.h"
#include "scene.h"
#include "res_mesh.h"
#include "res_shader.h"

struct Server
{
	entt::resource_cache<res_mesh> cl_meshes;
	entt::resource_cache<res_shader> cl_shaders;
	consoleInterpreter sv_interpreter;
	Scene sv_scene;
};