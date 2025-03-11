#pragma once
#include <SDL.h>
#include <glew_surrogate.h>
#include <filesystem>
#include <include/thirdparty/entt.hpp>
#include <include/GameContext.h>

int main() {
    Application* context = new GameContext;
	
	return context->run();

}

