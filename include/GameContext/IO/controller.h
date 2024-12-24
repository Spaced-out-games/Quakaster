#pragma once
#include "controllerEvents.h"
#include <include/thirdparty/entt.hpp>
#include <include/GameContext/server/ConsoleInterpreter.h>
#include <include/GameContext/UI/UIContext.h>

struct Controller
{
	// SDL event
	SDL_Event event;

	// For sending events
	eventHandler& event_handler;

	// For sending SDL events to the UI rather than this controller
	UIContext& ui_context;
	int lastMouseX = 0;
	int lastMouseY = 0;
	explicit Controller(eventHandler& event_handler, UIContext& ui_context) : event_handler(event_handler), ui_context(ui_context) {}

	void dispatchIOEvents()
	{
		while (SDL_PollEvent(&event))
		{
			if (event.key.keysym.sym == SDLK_ESCAPE && event.type == SDL_KEYDOWN) {
				ui_context.pause();
				//Client::current_client->console.open ^= 1;
			}
		}
	}


};