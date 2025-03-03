#pragma once
#include <SDL.h>
#include <GL/glew.h>
#include <GL/gl.h>
#include <filesystem>
#include <include/thirdparty/entt.hpp>
#include <iostream>
#include <include/GameContext/GameContext.h>
;
static inline std::string root_directory = "D:/Quakaster/";

int main() {
    GameContext context;
	return context.run();

}

