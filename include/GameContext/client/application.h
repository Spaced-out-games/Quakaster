#pragma once
#include <include/GameContext/window/window.h>
#include <include/GameContext/UI/UIContext.h>
#include <include/GameContext/IO/controller.h>

struct Application
{

	Window window;
	UIContext ui_context;


	Application(eventHandler& event_handler, ConsoleInterpreter& interpreter) : ui_context( event_handler, interpreter, window.get_renderer(), window)
	{
	}

};