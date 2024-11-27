

#pragma once
#include <vector>
#include <string>
#include <cstring>  // For `strcmp`
#include <iostream>
#include <unordered_map>
#include <functional>
#include <cstdlib>   // For std::strtol, std::strtof
#include <cctype>    // For std::isspace
#include <array>
#include <cmath>
#include <regex>
#include <imgui.h>
#include <backends/imgui_impl_sdl2.h>
#include <backends/imgui_impl_opengl3.h>

// Size of a single console command
#define CONSOLE_BUFFER_SIZE 256

// Which character is used to accent user input within the integrated console
#define CONSOLE_ACCENT_CHAR '~'

// A function that takes in a string (from the console) and does something with it. 
using console_function = std::function<void(std::string&)>;


/// <summary>
/// A wrapper for console_function to also include descriptions and example uses
/// </summary>
struct console_command
{
    // What the command does
    std::string description = "";

    // An example of using the command
    std::string example = "";

    // command to be run. Initializes to a command that does nothing
    console_function command = default_command;

    // constructor
    console_command(std::string description, std::string example, console_function command) :
        description(std::move(description)),
        example(std::move(example)),
        command(std::move(command))
    {}

    // constructor
    console_command() {}

    // Calls this command, passing the arguments
    void execute(std::string& arguments)
    {
        command(arguments);
    }
private:
    // no-op default function
    static void default_command(std::string& input) {}
};



class Scene;
class Application;





/// <summary>
/// ImGui in-Engine integrated console. Has a few commands like default that are no-brainers (eg, `echo`, `exit`, `clear`).
/// Inherit from thisand add more commands as needed.
/// 
/// TODO: Migrate commands to a server / level side interpreter, and have Console solely act as a means of getting a command from the CLI
/// </summary>
class Console
{
    friend Application;

    


    // Title of the console. 
    const char* title = "Integrated Console";
    // You can input up to 256 bytes into the console. 
    char input_buffer[CONSOLE_BUFFER_SIZE]{};

    // TODO: UNDERSTAND WHAT THIS DOES
    int history_pos = -1;

    // TODO: UNDERSTAND WHAT THIS DOES
    bool scrollToBottom = false;

    // Each logged command
    std::vector <std::string> history;

    // commands. Needs deprecated
    std::unordered_map<std::string, console_command> commands;

    protected:
        // leaves the pointer nullptr
        Console()
        {
            clear();
            log("Console initialized");
        }
        // A pointer to the scene that this Console can control
        Scene* my_scene = nullptr;

public:

    // Whether the Console is open or not
    bool open = true;

    // Making a console requires you define a scene for it to control. NOTE: might be a good idea to create a GameContext structure that keeps track of some more universal information, and pass a pointer to it in the constructor
    Console(Scene& scene);

    // Adds a line to the integrated console
    void log(const char* fmt, ...);

    // clears the integrated console
    void clear();

    // draws the console
    virtual void draw();

    // executes a command from the console
    virtual bool execute(const std::string& command);

    // Registers a command into the console. Might be better to eventually build something far more scalable, perhaps a tokenizer and interpreter
    void register_command(
        console_function func = [](std::string&) {},
        const std::string& name = "Unknown command",
        const std::string& description = "No description available",
        const std::string& example = ""

    )
    {
        commands[name] = console_command(description, example, func);
    }

    // Adds content to a pre-existing (or last) log entry
    void append_log(const std::string& content, int index);

    // Creates a new log
    void add_log(const std::string& content);
};

#include "scene.h"


Console::Console(Scene& scene) : history_pos(-1), scrollToBottom(false) {
    my_scene = &scene;
    clear();
    log("Console initialized");
}

void Console::log(const char* fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    history.emplace_back(fmt);  // Consider formatting here
    va_end(args);
}

void Console::clear()
{
    history.clear();
}

void Console::draw()
{
    if (!open) { return; }
    //std::cout << "console::draw\n";
    ImGui::SetNextWindowSize(ImVec2(720, 720), ImGuiCond_FirstUseEver);
    ImVec2 buttonSize = { 120, 20 };

    if (!ImGui::Begin(title, &open)) {
        ImGui::End();
        return;
    }

    if (ImGui::Button("Clear"))
        clear();

    ImGui::SameLine();
    bool copyToClipboard = ImGui::Button("Copy");
    ImGui::SameLine();
    ImVec2 windowSize = ImGui::GetWindowSize();
    ImGui::SetCursorPosX(windowSize.x - buttonSize.x - 10);
    if (ImGui::Button("Exit Application")) {
        exit(1);
    }

    ImGui::Separator();

    ImGui::BeginChild("ScrollingRegion", ImVec2(0, -ImGui::GetTextLineHeightWithSpacing() * 2), false, ImGuiWindowFlags_HorizontalScrollbar);
    if (copyToClipboard)
        ImGui::LogToClipboard();

    for (const auto& message : history)
        ImGui::TextUnformatted(message.c_str());

    if (scrollToBottom)
        ImGui::SetScrollHereY(1.0f);
    scrollToBottom = false;

    ImGui::EndChild();
    ImGui::Separator();

    ImGui::PushItemWidth(-1);
    if (ImGui::InputText("Input", input_buffer, IM_ARRAYSIZE(input_buffer), ImGuiInputTextFlags_EnterReturnsTrue)) {
        std::string inputStr(input_buffer);
        execute(inputStr);
        memset(input_buffer, 0, sizeof(input_buffer));
    }

    if (ImGui::IsItemHovered() || ImGui::IsWindowFocused(ImGuiFocusedFlags_RootAndChildWindows)) {
        ImGui::SetKeyboardFocusHere(1);
    }

    ImGui::PopItemWidth();
    ImGui::End();

}

bool Console::execute(const std::string& commandLine) {
    auto spaceIndex = commandLine.find(' ');
    std::string command = commandLine.substr(0, spaceIndex);
    std::string args = (spaceIndex != std::string::npos) ? commandLine.substr(spaceIndex + 1) : "";

    auto it = commands.find(command);
    if (it != commands.end()) {
        log(commandLine.c_str());
        it->second.execute(args);
        return true;
    }
    else {
        std::string message = "ERROR: Command '" + command + "' not found";
        log(message.c_str());
        return false;
    }
}

void Console::append_log(const std::string& content, int index)
{
    if (index == -1)
    {
        history[history.size() - 1] += content;
        return;
    }
    history[index] += content;
}
void Console::add_log(const std::string& content)
{
    history.emplace_back(content + "\n");
    scrollToBottom = true;
}
