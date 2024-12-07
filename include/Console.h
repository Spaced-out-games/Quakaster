

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
#include <sstream>
#include <imgui.h>
#include <backends/imgui_impl_sdl2.h>
#include <backends/imgui_impl_opengl3.h>

// Size of a single console command
#define CONSOLE_BUFFER_SIZE 256

// Which character is used to accent user input within the integrated console
#define CONSOLE_ACCENT_CHAR '~'

// A function that takes in a string (from the console) and does something with it. 
using console_function = std::function<void(std::string&)>;

#define DEFAULT_CONSOLE_COLOR 0


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


enum class console_colors : uint8_t
{
    WHITE = 0,
    DEFAULT_TEXT = WHITE,
    RED = 1,
    DEFAULT_ERROR_CRITICAL = RED,
    GREEN = 2,
    DEFAULT_OKAY = GREEN,
    YELLOW = 3,
    DEFAULT_WARNING = YELLOW,
    ORANGE = 4,
    DEFAULT_WARNING_SEVERE = ORANGE

};





struct console_message {
    console_colors colorIndex;  // Changed from uint8_t to console_colors
    std::string message;

    console_message(console_colors color = console_colors::DEFAULT_TEXT, const std::string& msg = "")
        : colorIndex(color), message(msg) {}

    // Overloading the += operator
    console_message& operator+=(const std::string& additionalMsg) {
        message += additionalMsg; // Append additional message
        return *this;
    }
};


class Scene;
class Application;

class Console;

// Event fired when the `enter` key was 
struct ConsoleCommandLineEvent
{
    ConsoleCommandLineEvent(Console& target, std::string& command) : target(&target), command(&command) {}
    Console* target;
    std::string* command;

    static void default_eventListener(ConsoleCommandLineEvent& event)
    {
        std::cout << (*event.command);
    }
};



/// <summary>
/// ImGui in-Engine integrated console. Has a few commands like default that are no-brainers (eg, `echo`, `exit`, `clear`).
/// Inherit from thisand add more commands as needed.
/// 
/// TODO: Migrate commands to a server / level side interpreter, and have Console solely act as a means of getting a command from the CLI
/// </summary>
class Console
{
    

    friend Application;
    
    ImU32 pallete[16]{};


    // Title of the console. 
    const char* title = "Integrated Console";
    // You can input up to 256 bytes into the console. 
    char input_buffer[CONSOLE_BUFFER_SIZE]{};

    // TODO: UNDERSTAND WHAT THIS DOES
    int history_pos = -1;

    // TODO: UNDERSTAND WHAT THIS DOES
    bool scrollToBottom = false;

    // Each logged command
    std::vector<console_message> history;

    // commands. Needs deprecated
    std::unordered_map<std::string, console_command> commands;

    ImU32 colorPalette[256];

    protected:
        // leaves the pointer nullptr
        Console();

        // A pointer to the scene that this Console can control
        Scene* my_scene = nullptr;

public:

    bool focused = false;

    // Whether the Console is open or not
    bool open = true;

    // Making a console requires you define a scene for it to control. NOTE: might be a good idea to create a GameContext structure that keeps track of some more universal information, and pass a pointer to it in the constructor
    Console(Scene& scene);

    // Adds a line to the integrated console
    void log(console_colors colorIndex, const char* fmt, ...);

    // clears the integrated console
    void clear();

    // draws the console
    virtual void draw();

    // executes a command from the console
    virtual bool execute(const std::string& command);

    virtual void init_commands() {};
    void register_default_commands();

    // Registers a command into the console. Might be better to eventually build something far more scalable, perhaps a tokenizer and interpreter
    void register_command( const std::string& name, const std::string& description, const std::string& example, console_function func = [](std::string&) {} )
    {
        commands[name] = console_command(description, example, func);
    }

    // Adds content to a pre-existing (or last) log entry
    void append_log(const std::string& content, int index);

    // Creates a new log
    void add_log(const console_message& msg);


    
};

#include "scene.h"



Console::Console(Scene& scene) : history_pos(-1), scrollToBottom(false) {
    my_scene = &scene;
    clear();
    register_default_commands();
    init_commands();
}

Console::Console()
{
    clear();
    register_default_commands();
    init_commands();

    colorPalette[static_cast<uint8_t>(console_colors::WHITE)] = IM_COL32(255, 255, 255, 255);    // White(White)
    colorPalette[static_cast<uint8_t>(console_colors::RED)] = IM_COL32(255, 0, 0, 255);            // Red

    colorPalette[static_cast<uint8_t>(console_colors::GREEN)] = IM_COL32(0, 150, 0, 255);          // Green(Green)

    colorPalette[static_cast<uint8_t>(console_colors::YELLOW)] = IM_COL32(255, 255, 0, 255);       // Yellow

    colorPalette[static_cast<uint8_t>(console_colors::ORANGE)] = IM_COL32(255, 165, 0, 255);
}

void Console::log(console_colors colorIndex, const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);

    char buffer[CONSOLE_BUFFER_SIZE];
    vsnprintf(buffer, sizeof(buffer), fmt, args);

    // Add the message to history with the specified color
    add_log(console_message(colorIndex, buffer));

    va_end(args);
}

void Console::clear()
{
    history.clear();
}

void Console::draw()
{
    if (!open) { return; }
    focused = false; // Reset focus status

    // Set window size and begin drawing the window
    ImGui::SetNextWindowSize(ImVec2(720, 720), ImGuiCond_FirstUseEver);

    ImVec2 buttonSize = { 120, 20 };
    if (!ImGui::Begin(title, &open)) {
        ImGui::End();
        return;
    }

    // Update focus status
    if (ImGui::IsWindowFocused(ImGuiFocusedFlags_RootAndChildWindows)) {
        focused = true;
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

    for (const auto& msg : history) {
        ImGui::PushStyleColor(ImGuiCol_Text, colorPalette[(uint8_t)msg.colorIndex]);
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

    // Input text for commands
    if (ImGui::InputText("Input", input_buffer, IM_ARRAYSIZE(input_buffer), ImGuiInputTextFlags_EnterReturnsTrue)) {
        std::string inputStr(input_buffer);

        my_scene->dispatcher.trigger<ConsoleCommandLineEvent>({ *this, inputStr });

        auto spaceIndex = inputStr.find(' ');
        std::string command = inputStr.substr(0, spaceIndex);

        // Check if the command exists
        auto it = commands.find(command);
        if (it != commands.end()) {
            log(console_colors::DEFAULT_OKAY, inputStr.c_str()); // Print the command
            it->second.execute(inputStr.substr(spaceIndex + 1)); // Execute with arguments
        }
        else {
            std::string errorMessage = "ERROR: Command '" + command + "' not found";
            log(console_colors::DEFAULT_WARNING, errorMessage.c_str()); // Print error message
        }

        memset(input_buffer, 0, sizeof(input_buffer));
        ImGui::SetKeyboardFocusHere(-1);
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
        log(console_colors::DEFAULT_TEXT,commandLine.c_str());
        it->second.execute(args);
        return true;
    }
    else {
        std::string message = "ERROR: Command '" + command + "' not found";
        log(console_colors::DEFAULT_WARNING,message.c_str());
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

void Console::add_log(const console_message& msg) {
    history.push_back(msg);
    scrollToBottom = true; // Ensure the console scrolls to the bottom
}

void Console::register_default_commands()
{
    register_command("exit", "Exits the application", "exit", [&](std::string& input) {
        exit(0);
    });

    register_command("help", "don't yell at me!", "exit", [&](std::string& input) {
        std::stringstream ss(input);
        std::string target_command;
        ss >> target_command;

        if (target_command.empty())
        {
            add_log({ console_colors::DEFAULT_TEXT,"Use help <command name> to learn more!" });
            return;
        }

        auto it = commands.find(target_command);

        if (it != commands.end()) {
            add_log({ console_colors::DEFAULT_TEXT, it->second.description });

        }
        else {
            std::string message = "ERROR: Command '" + target_command + "' not found";
            add_log({ console_colors::DEFAULT_TEXT, message.c_str() });

        }


        
    });

    register_command("clear", "Exits the application", "exit", [&](std::string& input) {
        exit(0);
    });

    register_command("debug", "Exits the application", "exit", [&](std::string& input) {
        exit(0);
    });
}





#include "application.h"


struct console_log_request {
    // Might be good to add some settings later
    //ImU32 color;
    std::string message;
    console_colors color_ID = console_colors::DEFAULT_TEXT;
    Console* target = nullptr;
    console_log_request(std::string msg, Console& console, console_colors color_ID = console_colors::DEFAULT_TEXT) : message(msg), target(&console), color_ID(color_ID) {}
    static void defaultEventListener(console_log_request& event) {
        event.target->log(event.color_ID, event.message.c_str());
    }
};

void console_log(std::string message, console_colors color_ID = console_colors::DEFAULT_TEXT)
{

    console_log_request console_message(message, Application::current_application->console, color_ID);
    EVENT_FIRE(Application::current_application->scene.dispatcher, console_log_request, console_message);
}

