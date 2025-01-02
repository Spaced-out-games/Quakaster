#pragma once
#include "controllerEvents.h"
#include <include/thirdparty/entt.hpp>
#include <include/GameContext/server/ConsoleInterpreter.h>
#include <include/GameContext/UI/UIContext.h>
#include <include/GameContext/IO/InputBase.h>

struct Controller
{
	// SDL event
	SDL_Event event;
	ImGuiIO* IOHandler;
	// For sending events
	eventHandler& event_handler;

	// For sending SDL events to the UI rather than this controller
	UIContext& ui_context;
	int lastMouseX = 0;
	int lastMouseY = 0;
	Controller(eventHandler& event_handler, UIContext& ui_context) : event_handler(event_handler), ui_context(ui_context)
	{

	}

	// Make sure ImGui is ready
	void init()
	{
		IOHandler = &ImGui::GetIO();
	}

	void dispatchIOEvents()
	{
		while (SDL_PollEvent(&event))
		{
			if (event.key.keysym.sym == SDLK_ESCAPE && event.type == SDL_KEYDOWN) {
				ui_context.elements[0]->visible ^= 1;
			}
		}
	}



};