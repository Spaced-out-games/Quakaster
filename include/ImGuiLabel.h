#pragma once
#include "IWidgetComponent.h"


class ImGuiLabel : public IWidgetComponent
{
public:
    ImGuiLabel(entt::dispatcher& dispatcher, const std::string& text)
        : IWidgetComponent(&ImGuiLabel::draw_impl), text(text), font(ImGui::GetFont()), dispatcher(dispatcher)
    {

    }

    void setFontSize(float size)
    {
        ImGui::SetWindowFontScale(size);
        // Here, you can also set a specific font if needed
        // Example: font = ImGui::GetFont(); // Or load a different font
    }

    void setFont(ImFont* newFont)
    {
        font = newFont;
    }

    void resetFont()
    {
        font = ImGui::GetFont(); // Reset to default font
        font_size = 16.0f;       // Reset to default size
    }

private:
    std::string text;      // The text to display
    float font_size = 2.5f;       // Font size
    ImFont* font;          // Pointer to the font
    entt::dispatcher& dispatcher;

    static void draw_impl(IWidgetComponent* component)
    {
        ImGuiLabel* label = static_cast<ImGuiLabel*>(component);

        // Set the font and font size for the label
        //ImGui::PushFont(label->font);
        //ImGui::SetWindowFontScale(label->font_size); // Scale the font based on the default size

        // Draw the label text
        ImGui::Text("%s", label->text.c_str());

        // Restore the original font and scaling
        //ImGui::PopFont();
    }
};

