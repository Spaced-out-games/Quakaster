#pragma once
#include <string>
#include <backends/imgui_impl_sdl2.h>
#include <backends/imgui_impl_opengl3.h>
#include "imgui.h"
#include <include/GameContext/server/ConsoleInterpreter.h>

// Size of a single console command
#define CONSOLE_BUFFER_SIZE 256

// Which character is used to accent user input within the integrated console
#define CONSOLE_ACCENT_CHAR '~'


#include "console_message.h"



struct UIContext;

struct ConsoleUI
{
    bool is_active(UIContext& context);
    ConsoleInterpreter interpreter;
    eventHandler& event_handler;
    ConsoleUI(ConsoleInterpreter& interpreter, eventHandler& event_handler) :
        interpreter(interpreter),
        event_handler(event_handler) {}
};

#include <include/GameContext/UI/UIContext.h>

bool ConsoleUI::is_active(UIContext& context)
{
    return context.
}