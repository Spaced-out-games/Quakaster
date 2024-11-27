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

	// consoleComponent's think script. After creating a Scene, register this function, and it will be called as part of the game system
	static void think(Scene& scene)
	{
		auto view = scene.view<consoleComponent>();
		for (auto entity : view)
		{
			std::string& content = view.get<consoleComponent>(entity).content;
			Console* console = view.get<consoleComponent>(entity).mConsole;
			console->add_log(content);

		}
	}

	// self explanatory constructor
	consoleComponent(const std::string& content, Console& console) : content(content), mConsole(&console) {}


};