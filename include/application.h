#pragma once
#include "Console.h"
#include "scene.h"
#include <SDL.h>
#include "window.h"




// Creates an application with a window, scene, and console. Lets the console know who's its daddy. 
struct Application
{

	// game window
	Window window;

	// game scene
	Scene scene;

	// integrated console
	Console console;


	// Creates an application and lets the console know about the scene it controls
	Application()
	{
		console.my_scene = &scene;
	}
};