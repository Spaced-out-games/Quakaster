#pragma once
#include "scene.h"
#include <iostream>


// Basic test component to make sure the ECS is working
struct textComponent
{

	// content to print to standalone console (std::cout)
	std::string content;

	// textComponent's think script. After creating a Scene, register this function, and it will be called as part of the game system
	static void think(Scene& scene)
	{
		auto view = scene.view<textComponent>();
		for (auto entity : view)
		{
			std::string& content = view.get<textComponent>(entity).content;
			std::cout << content;

		}
	}
	
	// self explanatory constructor
	textComponent(const std::string& content) : content(content) {}


};