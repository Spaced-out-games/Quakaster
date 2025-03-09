#pragma once
#include <SDL.h>
#include <glew_surrogate.h>
#include <filesystem>
#include <include/thirdparty/entt.hpp>
#include <include/GameContext.h>
;
static inline std::string root_directory = "D:/Quakaster/";

int main() {
    GameContext context;
	return context.run();

}

