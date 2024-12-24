#pragma once
#include "scene.h"
#include "Console.h"
#include <iostream>


// Basic test component to make sure the ECS is working
struct consoleComponent
{

	// content to print to an integrated console
	std::string content = "Text";
	Console* mConsole = nullptr;
	bool pending_destruction = false;

	// consoleComponent's think script. After creating a Scene, register this function, and it will be called as part of the game system
	static void think(Scene& scene)
	{
		auto view = scene.view<consoleComponent>();
		for (auto entity : view)
		{
			consoleComponent& console_component = view.get<consoleComponent>(entity);
			std::string& content = console_component.content;
			console_component.mConsole->add_log({ console_colors::DEFAULT_TEXT, content });
			if (console_component.pending_destruction)
			{
				scene.remove<consoleComponent>(entity);
			}
		}
	}

	// self explanatory constructor
	consoleComponent(const std::string& content, Console& console) : content(content), mConsole(&console) {}


};