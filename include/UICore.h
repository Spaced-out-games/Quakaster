#pragma once
#include <imgui.h>
#include <backends/imgui_impl_sdl2.h>
#include <backends/imgui_impl_opengl3.h>
#include "utility_types.h"
#include <stdexcept>
#include <cassert>
#include <memory> // For std::unique_ptr
#include <vector>
#include <unordered_map>
#include <typeindex>


namespace UI
{
    // Base widget component class
    struct IWidgetComponent
    {
        // Pointer to the draw function
        const func_ptr_t<void, IWidgetComponent*> draw_function;

        // Pass the draw function
        IWidgetComponent(func_ptr_t<void, IWidgetComponent*> draw_function = nullptr)
            : draw_function(draw_function)
        {
            if (draw_function == nullptr)
            {
                throw std::invalid_argument("draw_function cannot be null");
            }
        }

        // Basic wrapper for the draw function, with no overhead
        inline void draw()
        {
            draw_function(this);
        }
    };

    
    // An ImGui widget. Contains a list of components to draw
    struct UIWidget
    {
        bool is_open = true;
        bool is_focused = false;
        const char* title = "UI Widget"; // Example title
        std::vector<std::unique_ptr<IWidgetComponent>> components;
        //std::unordered_map<uint8_t, std::type_index> index_to_type;

        // Draws the widget. You don't need to call Imgui::Begin/end; that's done for you.
        void draw()
        {
            if (!is_open) return;

            if (!ImGui::Begin(title, &is_open))
            {
                ImGui::End();
                return;
            }

            for (const auto& component : components)
            {
                component->draw();
            }

            ImGui::End();
        }


        // Adds a component to the widget. Make sure you are adding instances of objects that derive from IWidgetComponent
        template <typename T, typename... Args>
        uint8_t add_component(Args&&... args)
        {
            static_assert(std::is_base_of<IWidgetComponent, T>::value, "T must derive from IWidgetComponent");

            auto component = std::make_unique<T>(std::forward<Args>(args)...);
            uint8_t index = static_cast<uint8_t>(components.size());
            //index_to_type[index] = std::type_index(typeid(T)); // Initialize type_index properly
            components.push_back(std::move(component));

            return index;
        }

        // Gets a component reference. 
        template <typename T>
        T& get_component(uint8_t index) const
        {

            return *(static_cast<T*>(components[index].get()));
        
            //return nullptr;
        }

    

    };

    // An embedded widget. 
    struct EmbeddedWidget
    {
        bool is_open = true;
        bool is_focused = false;
        const char* title = "UI Child"; // Example title
        std::vector<std::unique_ptr<IWidgetComponent>> components;
        ImVec2 size;
        ImGuiChildFlags  child_flags;
        ImGuiWindowFlags window_flags;

        //std::unordered_map<uint8_t, std::type_index> index_to_type;

        // Draws the widget. You don't need to call Imgui::Begin/end; that's done for you.
        void draw()
        {
            if (!is_open) return;

            if (ImGui::BeginChild(title, size, child_flags, window_flags))
            {
                ImGui::EndChild();
                return;
            }

            for (const auto& component : components)
            {
                component->draw();
            }

            ImGui::EndChild();
        }


        // Adds a component to the widget. Make sure you are adding instances of objects that derive from IWidgetComponent
        template <typename T, typename... Args>
        uint8_t add_component(Args&&... args)
        {
            static_assert(std::is_base_of<IWidgetComponent, T>::value, "T must derive from IWidgetComponent");

            auto component = std::make_unique<T>(std::forward<Args>(args)...);
            uint8_t index = static_cast<uint8_t>(components.size());
            //index_to_type[index] = std::type_index(typeid(T)); // Initialize type_index properly
            components.push_back(std::move(component));

            return index;
        }

        // Gets a component reference. 
        template <typename T>
        T& get_component(uint8_t index) const
        {

            return *(static_cast<T*>(components[index].get()));

            //return nullptr;
        }



    };

    // Updated Button
    struct Button : public IWidgetComponent
    {
        struct DefaultButtonPressedEvent
        {
            IWidgetComponent* button_component; // Reference to the button component
        };

        std::string label;
        entt::dispatcher& dispatcher; // Reference to the dispatcher
        func_ptr_t<void, IWidgetComponent*> on_press_function; // Function to call on press

        // Constructor takes the dispatcher and an optional press function
        Button(
            entt::dispatcher& dispatcher,
            const std::string& label,
            func_ptr_t<void, IWidgetComponent*> on_press_function = &Button::onButtonPress
        )
            : IWidgetComponent(&Button::draw),
            label(label),
            dispatcher(dispatcher),
            on_press_function(on_press_function) {}


    private:
        // Static draw function
        static void draw(IWidgetComponent* base)
        {
            auto* self = static_cast<Button*>(base);
            if (ImGui::Button(self->label.c_str()))
            {
                // Trigger the on_press_function if it is set
                if (self->on_press_function)
                {
                    self->on_press_function(self);
                }

                // Dispatch an event, if desired
                // Example of triggering an event (you can customize this)

            }
        }
        static void onButtonPress(IWidgetComponent* button)
        {
            auto* self = static_cast<Button*>(button);
            self->dispatcher.trigger<Button::DefaultButtonPressedEvent>({ self });
            console_log(std::string("Button pressed!"));
        }
    };

    // Example of a Checkbox component
    struct Checkbox : public IWidgetComponent
    {
        std::string label;
        bool value; // Pointer to a boolean value
        func_ptr_t<void, IWidgetComponent*> on_change_function; // Function to call on change

        Checkbox(const std::string& label, bool default_value,
            func_ptr_t<void, IWidgetComponent*> on_change_function = &Checkbox::onCheckboxChange)
            : IWidgetComponent(&Checkbox::draw),
            label(label),
            value(default_value),
            on_change_function(on_change_function) {}

    private:
        static void draw(IWidgetComponent* base)
        {
            auto* self = static_cast<Checkbox*>(base);
            if (ImGui::Checkbox(self->label.c_str(), &(self->value)))
            {
                // Trigger the on_change_function if it is set
                if (self->on_change_function)
                {
                    self->on_change_function(self);
                }
            }
        }
        static void onCheckboxChange(IWidgetComponent* checkbox)
        {
            auto* checkboxComp = static_cast<Checkbox*>(checkbox);
            console_log(std::string("Checkbox changed: ") + std::string(checkboxComp->value ? "Checked" : "Unchecked"));
        }
    };

    // Example of a Slider component
    struct Slider : public IWidgetComponent
    {
        std::string label;
        float value; // Pointer to a float value
        float min;
        float max;
        func_ptr_t<void, IWidgetComponent*> on_change_function; // Function to call on change

        Slider(const std::string& label, float default_value, float min, float max,
            func_ptr_t<void, IWidgetComponent*> on_change_function = &Slider::onSliderChange)
            : IWidgetComponent(&Slider::draw),
            label(label),
            value(default_value),
            min(min), max(max),
            on_change_function(on_change_function) {}

    private:
        static void draw(IWidgetComponent* base)
        {
            auto* self = static_cast<Slider*>(base);
            if (ImGui::SliderFloat(self->label.c_str(), &self->value, self->min, self->max))
            {
                // Trigger the on_change_function if it is set
                if (self->on_change_function)
                {
                    self->on_change_function(self);
                }
            }
        }
        static void onSliderChange(IWidgetComponent* slider)
        {
            auto* sliderComp = static_cast<Slider*>(slider);
            console_log(std::string("Slider value: ") + std::to_string(sliderComp->value));
        }
    };

    struct Separator : public IWidgetComponent
    {
        Separator()
            : IWidgetComponent(&Separator::draw) {}

    private:
        static void draw(IWidgetComponent* base)
        {
            ImGui::Separator();
        }
    };

    // A scrolling text region for displaying console messages
    struct ScrollingTextComponent : public IWidgetComponent
    {
        std::vector<std::pair<std::string, ImVec4>> messages; // Log messages with colors
        bool scroll_to_bottom = false;

        ScrollingTextComponent()
            : IWidgetComponent(&ScrollingTextComponent::draw) {}

        void add_message(const std::string& message, const ImVec4& color)
        {
            messages.emplace_back(message, color);
            scroll_to_bottom = true;
        }

    private:
        static void draw(IWidgetComponent* base)
        {
            auto* self = static_cast<ScrollingTextComponent*>(base);
            ImGui::BeginChild("ScrollingRegion", ImVec2(0, -ImGui::GetTextLineHeightWithSpacing() * 2), false, ImGuiWindowFlags_HorizontalScrollbar);

            for (const auto& [msg, color] : self->messages)
            {
                ImGui::PushStyleColor(ImGuiCol_Text, color);
                ImGui::TextUnformatted(msg.c_str());
                ImGui::PopStyleColor();
            }

            if (self->scroll_to_bottom)
                ImGui::SetScrollHereY(1.0f);

            self->scroll_to_bottom = false;
            ImGui::EndChild();
        }
    };
}