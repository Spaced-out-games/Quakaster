#pragma once
#include <cstdint>
#include <vector>
#include <include/GameContext/UI/UIBase.h>
#include <include/GameContext/server/ConsoleInterpreter.h> // For ConsoleInterpreter
#include <include/thirdparty/entt.hpp> // For eventHandler

// Forward declaration of ConsoleUI
struct ConsoleUI;



struct UIContext
{

    UIContext(eventHandler& event_handler, ConsoleInterpreter& interpreter);
    ~UIContext()
    {
        for (size_t i = 0; i < elements.size(); i++)
        {
            free(elements[i]);
        }
    }

    inline void draw()
    {
        for (size_t i = 0; i < elements.size(); i++)
        {
            if (elements[i]->visible)
            {
                elements[i]->draw();
            }
        }
    }

    inline void add_UIElement(UIBase* new_UIelement) { elements.push_back(new_UIelement); }


    // For handling events
    eventHandler& event_handler;

    // for interpreting console commands
    ConsoleInterpreter& interpreter;


    // UI Elements
    std::vector<UIBase*> elements;

    // Current element
    UIBase* current_element = nullptr;

    // Pointer to the console interface
    ConsoleUI* console;

    // Pauses to the console UI element
    void pause()
    {

    }
};

#include <include/GameContext/UI/ConsoleUI.h>

UIContext::UIContext(eventHandler& event_handler, ConsoleInterpreter& interpreter) : event_handler(event_handler), interpreter(interpreter)
{
    //ConsoleUI* console = new ConsoleUI{interpreter, event_handler, *this};
    add_UIElement(new ConsoleUI{ interpreter, event_handler, *this });
    //elements.emplace_back(console);
}