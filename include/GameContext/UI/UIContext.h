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

    // For handling events
    eventHandler& event_handler;

    ConsoleInterpreter& interpreter;
    // UI Elements
    std::vector<UIBase*> elements;

    // Current element
    UIBase* current_element;

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
    ConsoleUI* console = new ConsoleUI{interpreter, event_handler, *this};

    elements.emplace_back(console);
}