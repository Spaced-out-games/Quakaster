#pragma once
#include <include/thirdparty/entt.hpp>
#include "include/GameContext/window/window.h"
#include "include/GameContext/client/application.h"
#include <imgui.h>
#include <backends/imgui_impl_sdl2.h>
#include <backends/imgui_impl_opengl3.h>



struct GameContext
{
	bool running = true;
	ConsoleInterpreter interpreter;
	//(eventHandler& event_handler, ConsoleInterpreter& interpreter)
	GameContext() : app(event_handler, interpreter) {}
	eventHandler event_handler;
	Application app;
	virtual void run()
	{
		while (running)
		{
			app.ui_context.draw();
		}
	}
};