#pragma once
#include <cstdint>
#include <vector>
#include <include/GameContext/UI/UIBase.h>
#include <include/GameContext/server/interpreter/ConsoleInterpreter.h> // For ConsoleInterpreter
#include <include/GameContext/window/Renderer.h>

#include <include/common.h>

// Forward declaration of ConsoleUI
struct ConsoleUI;

using namespace Quakaster;

struct UIContext: public base::ISystem
{
    // -------------------------------------------------------- METHODS -------------------------------------------------------


    UIContext(EventHandler& event_handler, ConsoleInterpreter& interpreter, Renderer& renderer, Window& window);
    ~UIContext()
    {
        for (size_t i = 0; i < elements.size(); i++)
        {
            free(elements[i]);
        }
    }

    void init(base::Scene&) override {
        
    }


    // This ignores the Scene that needs passed
    void tick(base::Scene&) override {

        // assign to the controller by default
        for (size_t i = 0; i < elements.size(); i++)
        {
            if ((!paused && elements[i]->visible) || elements[i]->always_visible)
            {
                elements[i]->draw(this);
            }
        }
    }

    // In this instance, it does pretty much nothing.
    void destroy(base::Scene&) override {}

    // Pauses to the console UI element
    void pause() { paused = !paused; }

    inline void add_UIElement(UIBase* new_UIelement) { elements.push_back(new_UIelement); }

    
    // -------------------------------------------------------- MEMBERS -------------------------------------------------------

    // For handling events
    EventHandler& event_handler;

    // for interpreting console commands
    ConsoleInterpreter& interpreter;


    // UI Elements
    std::vector<UIBase*> elements;

    // Pointer to the console interface
    ConsoleUI* console;

    bool paused = 1;
    Renderer& renderer;
    Window& window;
    

    

    
};

#include <include/GameContext/UI/ConsoleUI.h>

UIContext::UIContext(EventHandler& event_handler, ConsoleInterpreter& interpreter, Renderer& renderer, Window& window) : event_handler(event_handler), interpreter(interpreter), renderer(renderer), window(window)
{
    add_UIElement(new ConsoleUI{ interpreter, event_handler, *this });
}