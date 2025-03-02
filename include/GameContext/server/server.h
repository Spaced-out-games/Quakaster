#pragma once
#include <include/thirdparty/entt.hpp>
#include <include/GameContext/server/interpreter/ConsoleInterpreter.h>
#include <include/GameContext/base/Scene.h>


struct Server
{
	ConsoleInterpreter interpreter;
	Scene scene;

};