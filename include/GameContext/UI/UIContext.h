#pragma once
#include <cstdint>
#include <vector>
#include <include/GameContext/UI/UIBase.h>
#include <include/GameContext/server/interpreter/ConsoleInterpreter.h> // For ConsoleInterpreter
#include <include/thirdparty/entt.hpp> // For eventHandler
#include <include/GameContext/window/Renderer.h>

// Forward declaration of ConsoleUI
struct ConsoleUI;



struct UIContext
{
    // InputBase* current_controller;
    bool free_mouse = 1;
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
        //if (free_mouse)
        //{
        //    return;
        //}
        // assign to the controller by default
        for (size_t i = 0; i < elements.size(); i++)
        {
            if (elements[i]->visible && !free_mouse || elements[i]->always_visible)
            {
                elements[i]->draw(this);
            }
        }





        //SDL_PollEvent(&evt);

        



        // Swap buffers, etc.
        //SDL_GL_SwapWindow(window.sdl_window);


    }

    template <typename T>
    T* get_UIElement() {
        static_assert(std::is_base_of<UIBase, T>::value, "Attempted to get a type that is not a widget");

        for (auto* element : elements) // Assuming elements is a container of pointers to UIBase
        {
            T* castedElement = dynamic_cast<T*>(element);
            if (castedElement) // Check if the cast was successful
            {
                return castedElement;
            }
        }
        return nullptr; // No element of type T found
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
        free_mouse = !free_mouse;
        //paused = !paused;
    }
};

#include <include/GameContext/UI/ConsoleUI.h>

UIContext::UIContext(eventHandler& event_handler, ConsoleInterpreter& interpreter, Renderer& renderer, Window& window) : event_handler(event_handler), interpreter(interpreter), renderer(renderer), window(window)
{
    
    
    
    //elements.emplace_back(console);
}