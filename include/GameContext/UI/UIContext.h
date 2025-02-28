#pragma once
#include <cstdint>
#include <vector>
#include <include/GameContext/UI/UIBase.h>
#include <include/GameContext/server/interpreter/ConsoleInterpreter.h> // For ConsoleInterpreter
#include <include/GameContext/base/eventHandler.h>
#include <include/GameContext/window/Renderer.h>

// Forward declaration of ConsoleUI
struct ConsoleUI;



struct UIContext
{
    // InputBase* current_controller;
    bool paused = 1;
    Renderer& renderer;
    Window& window;
    UIContext(eventHandler& event_handler, ConsoleInterpreter& interpreter, Renderer& renderer, Window& window);
    ~UIContext()
    {
        for (size_t i = 0; i < elements.size(); i++)
        {
            free(elements[i]);
        }
    }



    inline void draw()
    {
        if (paused)
        {
            return;
        }
        // assign to the controller by default
        for (size_t i = 0; i < elements.size(); i++)
        {
            if (elements[i]->visible)
            {
                elements[i]->draw(this);
            }
        }





        //SDL_PollEvent(&evt);

        



        // Swap buffers, etc.
        //SDL_GL_SwapWindow(window.sdl_window);


    }

    inline void add_UIElement(UIBase* new_UIelement) { elements.push_back(new_UIelement); }


    // For handling events
    eventHandler& event_handler;

    // for interpreting console commands
    ConsoleInterpreter& interpreter;


    // UI Elements
    std::vector<UIBase*> elements;

    // Current element
    //InputBase* current_element = nullptr;

    // Pointer to the console interface
    ConsoleUI* console;

    // Pauses to the console UI element
    void pause()
    {
        paused = !paused;
    }
};

#include <include/GameContext/UI/ConsoleUI.h>

UIContext::UIContext(eventHandler& event_handler, ConsoleInterpreter& interpreter, Renderer& renderer, Window& window) : event_handler(event_handler), interpreter(interpreter), renderer(renderer), window(window)
{
    add_UIElement(new ConsoleUI{ interpreter, event_handler, *this });
    
    
    
    //elements.emplace_back(console);
}