#pragma once
#include <cstdint>
#include <vector>
#include <include/UI/UIBase.h>
#include <include/server/interpreter/ConsoleInterpreter.h> // For ConsoleInterpreter
#include <include/window/Renderer.h>
#include <include/IO/InputDelegate.h>
#include <include/common.h>

// Forward declaration of ConsoleUI
struct ConsoleUI;

using namespace Quakaster;

struct UIContext: public base::ISystem
{
    // -------------------------------------------------------- METHODS -------------------------------------------------------


    UIContext(Renderer& renderer, Window& window, InputDelegate& input_delegate);
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
        ImGui_ImplSDL2_NewFrame();
        ImGui_ImplOpenGL3_NewFrame();
        ImGui::NewFrame();


        // assign to the controller by default
        for (size_t i = 0; i < elements.size(); i++)
        {
            if ((!is_paused() && elements[i]->visible) || elements[i]->always_visible)
            {
                elements[i]->draw(this);
            }
        }

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    }

    // In this instance, it does pretty much nothing.
    void destroy(base::Scene&) override {}

    // Pauses to the console UI element
    void pause() { input_delegate.togglePause(); }

    inline void add_UIElement(UIBase* new_UIelement) { elements.push_back(new_UIelement); }

    
    // -------------------------------------------------------- MEMBERS -------------------------------------------------------


    // for interpreting console commands
    //ConsoleInterpreter& interpreter;

    InputDelegate& input_delegate;

    //EventListener<PauseEvent> listener;

    // UI Elements
    std::vector<UIBase*> elements;

    // Pointer to the console interface
    ConsoleUI* console;

    //bool paused = 1;
    Renderer& renderer;
    Window& window;

    bool is_paused() {
        bool b = input_delegate.is_paused();
        return b;
    }

    
};

#include <include/UI/ConsoleUI.h>

UIContext::UIContext(Renderer& renderer, Window& window, InputDelegate& input_delegate) : renderer(renderer),
window(window), input_delegate(input_delegate)
{

    add_UIElement(new ConsoleUI{ *this });
}