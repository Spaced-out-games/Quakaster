/*#include "include/console.h"


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
    ImGui::SetNextWindowSize(ImVec2(720, 720), ImGuiCond_FirstUseEver);
    ImVec2 buttonSize = { 120, 19 };

    if (!ImGui::Begin(title, open)) {
        ImGui::End();
        return;
    }

    if (ImGui::Button("Clear"))
        ClearLog();

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
    if (ImGui::InputText("Input", inputBuf, IM_ARRAYSIZE(inputBuf), ImGuiInputTextFlags_EnterReturnsTrue)) {
        std::string inputStr(inputBuf);
        ExecuteCommand(inputStr);
        memset(inputBuf, 0, sizeof(inputBuf));
    }

    if (ImGui::IsItemHovered() || ImGui::IsWindowFocused(ImGuiFocusedFlags_RootAndChildWindows)) {
        ImGui::SetKeyboardFocusHere(1);
    }

    ImGui::PopItemWidth();
    ImGui::End();
    
}

void Console::execute(const std::string& command)
{
    auto spaceIndex = commandLine.find(' ');
    std::string command = commandLine.substr(0, spaceIndex);
    std::string args = (spaceIndex != std::string::npos) ? commandLine.substr(spaceIndex + 1) : "";

    auto it = commands.find(command);
    if (it != commands.end()) {
        AddLog(commandLine.c_str());
        it->second.execute(args);
        return true;
    }
    else {
        std::string message = "ERROR: Command '" + command + "' not found";
        AddLog(message.c_str());
        return false;
    }
}

void Console::append_log(const std::string& content, int index = -1)
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

*/