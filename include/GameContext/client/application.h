#pragma once
#include <include/GameContext/window/window.h>
#include <include/GameContext/UI/UIContext.h>

struct Application
{
	Application(eventHandler& event_handler, ConsoleInterpreter& interpreter) : ui_context( event_handler, interpreter, window.get_renderer() )
	{

	}
	Window window;
	UIContext ui_context;
};