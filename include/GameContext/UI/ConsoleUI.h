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
#include <include/GameContext/UI/console_message.h>

// Size of a single console command
#define CONSOLE_BUFFER_SIZE 256
#define CONSOLE_BUFFER_SIZE 256

// Which character is used to accent user input within the integrated console
#define CONSOLE_ACCENT_CHAR '~'

struct UIContext;

struct ConsoleUI: public UIBase
{
    // communication network with components

    ConsoleInterpreter& interpreter;
    eventHandler& event_handler;
    UIContext& ui_context;


    // personal actual members
    char* title = "Integrated Console";

    // buffer for storing console input
    char input_buffer[CONSOLE_BUFFER_SIZE]{};
    
    // position to insert at (?)
    int history_pos = -1;

    // whether to automatically scroll to the bottom after entering a command
    bool scrollToBottom = false;

    std::vector<console_message> history;

    ImU32 colorPallete[16];
    
    

    void add_log(const console_message& consoleMsg);

    inline void clear() { history.clear(); }








    ConsoleUI(ConsoleInterpreter& interpreter, eventHandler& event_handler, UIContext& ui_context) :
        interpreter(interpreter),
        event_handler(event_handler),
        ui_context(ui_context) {}
    void draw() override
    {
        //if (!visible) return;

        //ImGui::ShowDemoWindow();

        // reset focus to nullptr
        


        // start a new window, and don't have it be contingent on a pre-existing .ini
        ImGui::SetNextWindowSize({ 720, 720 }, false);

        ImVec2 buttonSize = { 120, 20 };

        // Try to begin ImGui, and if it fails, return early
        if (!ImGui::Begin(title, &visible))
        {
            std::cout << "Begin Failed";
            ImGui::End();
            return;
        }
        else
        {
            
        }




        // Add clear button
        if (ImGui::Button("Clear"))
            clear();
        ImGui::SameLine();
        bool line_copied = ImGui::Button("Copy");
        ImGui::SameLine();

        ImVec2 windowSize = ImGui::GetWindowSize();
        ImGui::SetCursorPosX(windowSize.x - buttonSize.x - 10);
        if (ImGui::Button("Exit Application")) {
            exit(1);
        }
        ImGui::Separator();

        ImGui::BeginChild("ScrollingRegion", ImVec2(0, -ImGui::GetTextLineHeightWithSpacing() * 2), false, ImGuiWindowFlags_HorizontalScrollbar);
        if (line_copied)
            ImGui::LogToClipboard();

        // rendering the console history
        for (const auto& msg : history) {
            
            ImGui::PushStyleColor(ImGuiCol_Text, colorPallete[(uint8_t)msg.color]);
            
            ImGui::TextUnformatted(msg.message.c_str());
            ImGui::PopStyleColor();
        }

        if (scrollToBottom)
            ImGui::SetScrollHereY(1.0f);
        scrollToBottom = false;

        ImGui::EndChild();
        ImGui::Separator();

        ImGui::PushItemWidth(-1);

        // Focus the input text box when the window is appearing
        if (ImGui::IsWindowAppearing()) {
            ImGui::SetKeyboardFocusHere(); // Focus the next item (the InputText)
        }


        if (ImGui::InputText("Input", input_buffer, IM_ARRAYSIZE(input_buffer), ImGuiInputTextFlags_EnterReturnsTrue)) {
            std::string inputStr(input_buffer);
            // this is where you would fire an event




        }
        ImGui::End();

    }

};


