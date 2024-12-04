#pragma once
#include "IWidgetComponent.h"

// Custom event type for button press
struct ButtonPressedEvent
{
    std::string button_label; // Store button label for identification

    ButtonPressedEvent(const std::string& label) : button_label(label) {}
};

// ImGuiButton class definition
class ImGuiButton : public IWidgetComponent
{
public:
    ImGuiButton(entt::dispatcher& dispatcher, const std::string& label)
        : IWidgetComponent(&ImGuiButton::draw_impl), label(label), dispatcher(dispatcher)
    {
        // Register the think script with the dispatcher and store the connection
        connection = dispatcher.sink<ButtonPressedEvent>().connect<&ImGuiButton::think>();
    }

    // Override draw method to render ImGui Button
    static void draw_impl(IWidgetComponent* component)
    {
        ImGuiButton* button = static_cast<ImGuiButton*>(component);

        if (ImGui::Button(button->label.c_str()))
        {
            button->fire_event(); // Fire event when the button is pressed
        }
    }

    ~ImGuiButton()
    {
        // Unregister the think script from the dispatcher
        connection.release(); // Correctly use disconnect
    }

private:
    std::string label; // Button label
    entt::dispatcher& dispatcher; // Reference to the dispatcher
    entt::connection connection; // Store the connection to allow disconnection

    void fire_event()
    {
        // Fire a custom event when the button is pressed
        dispatcher.trigger<ButtonPressedEvent>(label); // Example event
    }

    // Static think script to process ButtonPressedEvent
    static void think(const ButtonPressedEvent& event)
    {
        // Add logic for handling the button press here
        console_log(std::string("Button pressed with label '" + event.button_label + "'\n"));
    }
};
