#include <string>
#include <imgui.h>

class UIObject {
protected:
    std::string title; // Window title
    bool isFocused = false; // Focus state
    bool isOpen = true; // Window open/close state

public:
     UIObject(std::string title = "UIObject") : title(title) {}

    virtual ~UIObject() = default;

    virtual void draw()
    {
        if (!isOpen) { return; }
        isFocused = false;
        // Set window size and begin drawing the window
        ImGui::SetNextWindowSize(ImVec2(720, 720), ImGuiCond_FirstUseEver);

        ImVec2 buttonSize = { 120, 20 };
        if (!ImGui::Begin(title.c_str() , &isOpen)) {
            ImGui::End();
            return;
        }

        ImGui::End();
    }
    // Accessors for focus and open state
    bool getFocusState() const { return isFocused; }
    bool isOpenState() const { return isOpen; }
};

