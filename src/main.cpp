#pragma once
#include <SDL.h>
#include <GL/glew.h>
#include <GL/gl.h>
#include <include/thirdparty/entt.hpp>
#include <iostream>
#include <include/GameContext/GameContext.h>


#include <include/GameContext/IO/controller.h>

#include <include/GameContext/client/Client.h>

int main() {
    GameContext context;
	context.run();

}