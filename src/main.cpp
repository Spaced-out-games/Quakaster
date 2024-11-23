#define SDL_MAIN_HANDLED
#define GLEW_STATIC  // If you're linking against static GLEW library
#include <GL\glew.h>// Use GLEW or another OpenGL loader

#include <SDL.h>
#include <iostream>
#include "entity.h"
#include "ostream_operators.h"
#include "position.h"




int main()
{
	entt::entity t = entity();
	movable_2D::add_components(t);
	std::cout << "test worked";
}