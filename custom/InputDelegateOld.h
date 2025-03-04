#pragma once
#include "InputEvents.h"
#include <include/base/eventHandler.h>

#include <include/server/interpreter/ConsoleInterpreter.h>
#include <include/UI/UIContext.h>
#include <include/utils/utility_types.h>
#include <unordered_map>

/*

Added some QOL changes to GUI so that opening the GUI would automatically open some widgets. Needs streamlined, though.

Note to self: on pause, set an index to -1, and have each GUI component have its index passed when drawing. The GUI element will set the Context's index pointer to itself, and setting focus. Ideally, each widget should have a priority integer so that it's more controllable and not contingent on ordering
*/

struct InputDelegate
{
    // SDL event
    SDL_Event event;
    ImGuiIO* IOHandler = nullptr;
    UIContext& ui_context;        // UI context
    EventHandler& dispatcher; // Event dispatcher
    int lastMouseX = 0;
    int lastMouseY = 0;

    std::unordered_map<int, bool> keyStates;


    // Constructor
    InputDelegate(UIContext& ui_context, EventHandler& dispatcher)
        : ui_context(ui_context), dispatcher(dispatcher)
    {
        if (isImGuiInitialized())
        {
            init();
        }
    }

    bool isImGuiInitialized() const
    {
        return ImGui::GetCurrentContext() != nullptr;
    }

    // Initialize ImGui IO
    void init()
    {
        IOHandler = &ImGui::GetIO();
    }

    // Update controller input and forward events
    void update()
    {


        // Only draw if in relative mode
        ui_context.paused = ui_context.paused;
        // Ensure ImGui is ready
        if (!IOHandler)
            init();


        int xrel = 0;
        int yrel = 0;
        Uint16 mod;



        // Poll and process SDL events
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE)
            {
                // Toggle the relative mode
                ui_context.paused = !ui_context.paused; // Toggle the state

                // this will cause errors
                ConsoleUI* consoleUI = dynamic_cast<ConsoleUI*>(ui_context.elements.at(0));

                if (!ui_context.paused) { consoleUI->last_pause_state = 0; }


                SDL_SetRelativeMouseMode(ui_context.paused ? SDL_TRUE : SDL_FALSE); // Set relative mouse mode
                dispatcher.trigger(KeyPressEvent{ event.key.keysym.sym, event.key.keysym.mod });
                return; // Skip further processing for this event
            }

            // Forward the event to ImGui
            ImGui_ImplSDL2_ProcessEvent(&event);

            // Handle events not captured by ImGui
            if ((!IOHandler->WantCaptureKeyboard || !IOHandler->WantCaptureMouse) && ui_context.paused)
            {
                switch (event.type)
                {
                case SDL_KEYDOWN:
                    dispatcher.trigger(KeyPressEvent{ event.key.keysym.sym, event.key.keysym.mod });
                    keyStates[static_cast<int>(event.key.keysym.sym)] = 1;
                    break;

                case SDL_KEYUP:
                    dispatcher.trigger(KeyReleaseEvent{ event.key.keysym.sym, event.key.keysym.mod });


                    keyStates.erase(static_cast<int>(event.key.keysym.sym));

                    break;

                case SDL_MOUSEBUTTONDOWN:
                    //dispatcher.trigger(MouseClickEvent{
                        //event.button.button, event.button.x, event.button.y, true });
                    break;

                case SDL_MOUSEBUTTONUP:
                    //dispatcher.trigger(MouseClickEvent{
                        //event.button.button, event.button.x, event.button.y, false });
                    break;

                case SDL_WINDOWEVENT:
                    dispatcher.trigger(WindowEvent{ event.window.event });
                    break;

                case SDL_MOUSEMOTION:
                    if (ui_context.paused)
                    {
                        // Get mouse motion details
                        lastMouseX = event.motion.x;
                        lastMouseY = event.motion.y;
                        xrel = event.motion.xrel; // Change in x position
                        yrel = event.motion.yrel; // Change in y position
                        mod = event.motion.state; // Modifier keys

                        // Trigger mouse move event
                        dispatcher.trigger(MouseMoveEvent{ lastMouseX, lastMouseY, xrel, yrel, mod });

                        // Update ImGui mouse position
                        IOHandler->MousePos = ImVec2(static_cast<float>(lastMouseX), static_cast<float>(lastMouseY));
                        break;
                    }


                default:
                    break;
                }
            }
        }

        if (!IOHandler->WantCaptureKeyboard || !IOHandler->WantCaptureMouse)
        {
            if (!IOHandler->WantCaptureKeyboard || !IOHandler->WantCaptureMouse)
            {
                for (const auto& [key, isPressed] : keyStates)
                {
                    if (isPressed)
                    {
                        // Trigger a repeat event for the pressed key
                        dispatcher.trigger(KeyHoldEvent{ key });
                    }
                }
            }
        }
    }
};


enum event_type_mask : uint8_t {
    KEY_PRESS = 1 << 1,
    KEY_HOLD = 1 << 2,
    KEY_RELEASE = 1 << 3,
    MOUSE_PRESS = 1 << 4,
    MOUSE_RELEASE = 1 << 5,
    MOUSE_MOVE = 1 << 6,
    MOUSE_SCROLL = 1 << 7,
    WINDOW_EVENT = 1 << 8,

    KEY_EVENTS = KEY_PRESS | KEY_RELEASE | KEY_HOLD,
    MOUSE_EVENTS = MOUSE_PRESS | MOUSE_RELEASE | MOUSE_MOVE | MOUSE_SCROLL,
    ALL_EVENTS = KEY_EVENTS | MOUSE_EVENTS | WINDOW_EVENT // more explicit than 0xff
};

#include <functional> // Include for std::function
#include <iostream>   // Include for std::cout (optional, for demonstration)

struct EventListener {
    const event_type_mask mask;
    std::function<void(KeyPressEvent&)> on_keyPress = [](KeyPressEvent&) {};
    std::function<void(KeyHoldEvent&)> on_keyHold = [](KeyHoldEvent&) {};
    std::function<void(KeyReleaseEvent&)> on_keyRelease = [](KeyReleaseEvent&) {};

    std::function<void(MouseClickEvent&)> on_mousePress = [](MouseClickEvent&) {};
    std::function<void(MouseReleaseEvent&)> on_mouseRelease = [](MouseReleaseEvent&) {};
    std::function<void(MouseMoveEvent&)> on_mouseMove = [](MouseMoveEvent&) {};
    std::function<void(MouseScrollEvent&)> on_mouseScroll = [](MouseScrollEvent&) {};

    std::function<void(WindowEvent&)> on_windowEvent = [](WindowEvent&) {};

    EventListener(EventHandler& dispatcher, event_type_mask mask) : mask(mask)
    {
        if (mask & event_type_mask::KEY_PRESS) {
            dispatcher.sink<KeyPressEvent>().connect<&EventListener::onKeyPress>(this);
        }
        if (mask & event_type_mask::KEY_HOLD) {
            dispatcher.sink<KeyHoldEvent>().connect<&EventListener::onKeyHold>(this);
        }
        if (mask & event_type_mask::KEY_RELEASE) {
            dispatcher.sink<KeyReleaseEvent>().connect<&EventListener::onKeyRelease>(this);
        }
        if (mask & event_type_mask::MOUSE_PRESS) {
            dispatcher.sink<MouseClickEvent>().connect<&EventListener::onMousePress>(this);
        }
        if (mask & event_type_mask::MOUSE_RELEASE) {
            dispatcher.sink<MouseReleaseEvent>().connect<&EventListener::onMouseRelease>(this);
        }
        if (mask & event_type_mask::MOUSE_MOVE) {
            dispatcher.sink<MouseMoveEvent>().connect<&EventListener::onMouseMove>(this);
        }
        if (mask & event_type_mask::MOUSE_SCROLL) {
            dispatcher.sink<MouseScrollEvent>().connect<&EventListener::onMouseScroll>(this);
        }
        if (mask & event_type_mask::WINDOW_EVENT) {
            dispatcher.sink<WindowEvent>().connect<&EventListener::onWindowEvent>(this);
        }
    }

private:
    void onKeyPress(KeyPressEvent& evt) {
        on_keyPress(evt);
    }

    void onKeyHold(KeyHoldEvent& evt) {
        on_keyHold(evt);
    }

    void onKeyRelease(KeyReleaseEvent& evt) {
        on_keyRelease(evt);
    }

    void onMousePress(MouseClickEvent& evt) {
        on_mousePress(evt);
    }

    void onMouseRelease(MouseReleaseEvent& evt) {
        on_mouseRelease(evt);
    }

    void onMouseMove(MouseMoveEvent& evt) {
        on_mouseMove(evt);
    }

    void onMouseScroll(MouseScrollEvent& evt) {
        on_mouseScroll(evt);
    }

    void onWindowEvent(WindowEvent& evt) {
        on_windowEvent(evt);
    }
};



