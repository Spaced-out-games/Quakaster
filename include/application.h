#pragma once

#include "Console.h"
#include "scene.h"
#include <SDL.h>
#include "window.h"

// Creates an application with a window, scene, and console. Lets the console know who's its daddy. 

struct Application
{
    bool isRunning = true;

    // In case you are only using one application (which you probably are) and want to do whatever from anywhere. 
    static Application* current_application;

    // game window
    Window window;

    // game scene
    Scene scene;

    // integrated console
    Console console;

    // Creates an application and lets the console know about the scene it controls. Also calls the bootstrap function
    // and begins the running of the application
    Application()
    {
        console.my_scene = &scene;
        scene.owner = this;
        current_application = this;
    }

    // Correctly declare pure virtual functions
    func_ptr_t<void, Application&> bootstrap = nullptr;
    func_ptr_t<void, Application&> tick = nullptr;


    void run()
    {
        bootstrap(*this);
        while (isRunning)
        {
            tick(*this);

        }
    }
    template<typename T>
    T& cast()
    {
        static_assert(std::is_base_of<Application, T>::value, "Attempted to cast to a type not derivative of Application");
        return *(T*)(this);

    }
};
Application* Application::current_application = nullptr;