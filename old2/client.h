#pragma once
#include "entt.hpp"
#include "scene.h"
#include "controller.h"
#include "res_mesh.h"
#include "res_shader.h"
#include "Console.h"
// #include "res_texture.h"

class Client
{
	public:

	entt::resource_cache<res_mesh> cl_meshes;
	entt::resource_cache<res_shader> cl_shaders;
	Scene cl_scene;
	Controller cl_controller;
	Console cl_console;
	Server& sv_server; // Replace this with a socket handle or whatever else in the future

	Client(Server& server) : sv_server(server) {}

};