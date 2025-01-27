#pragma once
#include <include/GameContext/window/window.h>
#include <include/GameContext/UI/UIContext.h>
#include <include/GameContext/IO/InputDelegate.h>

// Basic application structure. Contains a window and a UIContext (built on ImGUI)
struct Application
{
	
	Window window;
	
	// Contains all the UI widgets on the screen and draws them all using draw().
	UIContext ui_context;

	// TODO: Remove the ConsoleInterpreter dependency; remove the automatic console construction to meet this requirement; add some methods for getting the first instance, using static_cast

	// Constructor, takes in a entt::dispatcher& for dispatching events, and a consoleInterpreter& for telling the built-in console where to send its commands after tokenization
	Application(eventHandler& event_handler, ConsoleInterpreter& interpreter) : ui_context( event_handler, interpreter, window.get_renderer(), window), window(interpreter)
	{
	}

};