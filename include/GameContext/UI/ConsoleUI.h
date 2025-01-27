#pragma once
#include <string>
#include <backends/imgui_impl_sdl2.h>
#include <backends/imgui_impl_opengl3.h>
#include "imgui.h"
#include <include/GameContext/server/interpreter/ConsoleInterpreter.h>
#include "console_message.h"
#include <include/GameContext/UI/UIContext.h> // Include UIContext before implementing methods
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
    entt::dispatcher& event_handler;
    UIContext& ui_context;


    // personal actual members
    const char* title = "Integrated Console";

    // buffer for storing console input
    char input_buffer[CONSOLE_BUFFER_SIZE]{};
    
    // position to insert at (?)
    int history_pos = -1;

    // whether to automatically scroll to the bottom after entering a command
    bool scrollToBottom = false;

    std::vector<console_message> history;
    bool last_pause_state = 0;




    ImU32 colorPalette[16]{};
    
    

    void add_log(const console_message& consoleMsg)
    {
        history.push_back(consoleMsg);
    }

    inline void clear() { history.clear(); }


    void add_colors()
    {
        colorPalette[static_cast<uint8_t>(console_color::WHITE)] = IM_COL32(255, 255, 255, 255);    // White(White)

        colorPalette[static_cast<uint8_t>(console_color::RED)] = IM_COL32(255, 0, 0, 255);            // Red

        colorPalette[static_cast<uint8_t>(console_color::GREEN)] = IM_COL32(0, 150, 0, 255);          // Green(Green)

        colorPalette[static_cast<uint8_t>(console_color::YELLOW)] = IM_COL32(255, 255, 0, 255);       // Yellow

        colorPalette[static_cast<uint8_t>(console_color::ORANGE)] = IM_COL32(255, 165, 0, 255);
    }





    ConsoleUI(ConsoleInterpreter& interpreter, entt::dispatcher& event_handler, UIContext& ui_context) :
        interpreter(interpreter),
        event_handler(event_handler),
        ui_context(ui_context)
    {
        event_handler.sink<console_message>().connect<&ConsoleUI::callback>(this);


        add_colors();
    }
    void callback(console_message& msg) {
        this->add_log(msg);
    }

    void draw(UIContext* ctx) override
    {
        

        //if (!visible) return;

        // reset focus to nullptr
        


        // start a new window, and don't have it be contingent on a pre-existing .ini
        ImGui::SetNextWindowSize({ 720, 720 }, false);

        ImVec2 buttonSize = { 120, 20 };

        // Try to begin ImGui, and if it fails, return early
        if (!ImGui::Begin(title, &visible))
        {
            std::cout << "Begin Failed";
            ImGui::End();
            //last_pause_state = ui_context.free_mouse;
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
            

            ImGui::PushStyleColor(ImGuiCol_Text, colorPalette[(uint8_t)msg.color]);
            
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
        //if (ImGui::IsWindowAppearing()) {
        
        // last_pause_state = ui_context.free_mouse;

        if (last_pause_state == 0 && ui_context.free_mouse == 0)
        {
            // this is supposed to be called when the console opens
            ImGui::SetKeyboardFocusHere(); // Focus the next item (the InputText)
            last_pause_state = 1;
        }
        //}


        if (ImGui::InputText("Input", input_buffer, IM_ARRAYSIZE(input_buffer), ImGuiInputTextFlags_EnterReturnsTrue)) {
            std::string inputStr(input_buffer);
            console_message message = { "", console_color::DEFAULT_TEXT };
            interpreter.execute(inputStr, message);
            if(message.message != "") add_log(message);
            memset(input_buffer, 0, sizeof(input_buffer));
            last_pause_state = 0;



        }
        ImGui::End();

        //last_pause_state = ui_context.free_mouse;
    }

};


