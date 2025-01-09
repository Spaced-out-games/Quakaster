#pragma once
#include <SDL.h>
#include <GL/glew.h>
#include <GL/gl.h>
#include <filesystem>
#include <include/thirdparty/entt.hpp>
#include <iostream>
#include <include/GameContext/GameContext.h>
//#include <glm/glm.hpp>

#include <include/GameContext/IO/controller.h>
#include <include/GameContext/client/Client.h>

static std::string root_directory = "D:/Quakaster/";

int main() {
    GameContext context;
	context.run();

}

