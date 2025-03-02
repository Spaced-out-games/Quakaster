#pragma once
#include <cstdint>
#include <vector>
#include <include/GameContext/UI/UIBase.h>
#include <include/GameContext/server/interpreter/ConsoleInterpreter.h> // For ConsoleInterpreter
#include <include/GameContext/window/Renderer.h>
#include <include/GameContext/IO/InputDelegate.h>
#include <include/common.h>

// Forward declaration of ConsoleUI
struct ConsoleUI;

using namespace Quakaster;

struct UIContext: public base::ISystem
{
    // -------------------------------------------------------- METHODS -------------------------------------------------------


    UIContext(EventHandler& event_handler, ConsoleInterpreter& interpreter, Renderer& renderer, Window& window, InputDelegate& input_delegate);
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
            if ((!is_paused() && elements[i]->visible) || elements[i]->always_visible)
            {
                elements[i]->draw(this);
            }
        }
    }

    // In this instance, it does pretty much nothing.
    void destroy(base::Scene&) override {}

    // Pauses to the console UI element
    void pause() { input_delegate.is_paused ^= 1; }

    inline void add_UIElement(UIBase* new_UIelement) { elements.push_back(new_UIelement); }

    
    // -------------------------------------------------------- MEMBERS -------------------------------------------------------

    // For handling events
    EventHandler& event_handler;

    // for interpreting console commands
    ConsoleInterpreter& interpreter;

    InputDelegate& input_delegate;

    EventListener<PauseEvent> listener;

    // UI Elements
    std::vector<UIBase*> elements;

    // Pointer to the console interface
    ConsoleUI* console;

    //bool paused = 1;
    Renderer& renderer;
    Window& window;

    bool is_paused() { return input_delegate.is_paused; }

    
};

#include <include/GameContext/UI/ConsoleUI.h>

UIContext::UIContext(EventHandler& event_handler, ConsoleInterpreter& interpreter, Renderer& renderer, Window& window, InputDelegate& input_delegate) : event_handler(event_handler), interpreter(interpreter),
renderer(renderer),
window(window), input_delegate(input_delegate),
listener(event_handler, [this](PauseEvent) {
    pause();
    ConsoleUI* consoleUI = dynamic_cast<ConsoleUI*>(elements.at(0));
    if (!is_paused()) consoleUI->last_pause_state = 0;
    SDL_SetRelativeMouseMode(is_paused() ? SDL_TRUE : SDL_FALSE);  // Set relative mouse mode



    
})
{

    add_UIElement(new ConsoleUI{ interpreter, event_handler, *this });
}