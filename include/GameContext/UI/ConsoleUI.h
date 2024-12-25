#pragma once
#include <string>
#include <backends/imgui_impl_sdl2.h>
#include <backends/imgui_impl_opengl3.h>
#include "imgui.h"
#include <include/GameContext/server/ConsoleInterpreter.h>
#include "console_message.h"
#include <include/GameContext/UI/UIContext.h> // Include UIContext before implementing methods
#include <include/GameContext/IO/InputBase.h>
#include <include/GameContext/UI/UIBase.h>

// Size of a single console command
#define CONSOLE_BUFFER_SIZE 256

// Which character is used to accent user input within the integrated console
#define CONSOLE_ACCENT_CHAR '~'

struct UIContext;

struct ConsoleUI: public InputBase, public UIBase
{
    ConsoleInterpreter& interpreter;
    eventHandler& event_handler;
    UIContext& ui_context;
    ConsoleUI(ConsoleInterpreter& interpreter, eventHandler& event_handler, UIContext& ui_context) :
        interpreter(interpreter),
        event_handler(event_handler),
        ui_context(ui_context) {}
    void draw() override {}
    void on_event(const SDL_Event& event) override
    {
        ImGui_ImplSDL2_ProcessEvent(&event);
    }
};


