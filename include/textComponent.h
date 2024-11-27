#pragma once
#include "scene.h"
#include <iostream>

struct textComponent
{
	std::string content;
	static void think(Scene& scene)
	{
		auto view = scene.view<textComponent>();
		for (auto entity : view)
		{
			std::string& content = view.get<textComponent>(entity).content;
			std::cout << content;

		}
	}
	textComponent(const std::string& content) : content(content) {}

};