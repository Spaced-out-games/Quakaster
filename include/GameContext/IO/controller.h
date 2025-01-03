#pragma once
#include "controllerEvents.h"
#include <include/thirdparty/entt.hpp>
#include <include/GameContext/server/interpreter/ConsoleInterpreter.h>
#include <include/GameContext/UI/UIContext.h>
#include <include/GameContext/IO/InputBase.h>
#include <include/GameContext/utils/utility_types.h>

struct Controller
{
    // SDL event
    SDL_Event event;
    ImGuiIO* IOHandler = nullptr;
    eventHandler& event_handler;  // Custom event handler
    UIContext& ui_context;        // UI context
    entt::dispatcher& dispatcher; // Event dispatcher
    int lastMouseX = 0;
    int lastMouseY = 0;

    // Constructor
    Controller(eventHandler& event_handler, UIContext& ui_context, entt::dispatcher& dispatcher)
        : event_handler(event_handler), ui_context(ui_context), dispatcher(dispatcher)
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
        // Ensure ImGui is ready
        if (!IOHandler)
            init();

        // Poll and process SDL events
        while (SDL_PollEvent(&event))
        {
            // Forward the event to ImGui
            ImGui_ImplSDL2_ProcessEvent(&event);

            // Handle events not captured by ImGui
            if (!IOHandler->WantCaptureKeyboard || !IOHandler->WantCaptureMouse)
            {
                switch (event.type)
                {
                case SDL_KEYDOWN:
                    dispatcher.trigger(KeyPressEvent{ event.key.keysym.sym, event.key.keysym.mod });
                    break;

                case SDL_KEYUP:
                    dispatcher.trigger(KeyReleaseEvent{ event.key.keysym.sym, event.key.keysym.mod });
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

                default:
                    break;
                }
            }
        }
    }
};


enum event_type_mask : uint8_t {
    KEY_PRESS = 0x01,
    KEY_HOLD = 0x02,
    KEY_RELEASE = 0x04,
    MOUSE_PRESS = 0x08,
    MOUSE_RELEASE = 0x10,
    MOUSE_MOVE = 0x20,
    MOUSE_SCROLL = 0x40,
    WINDOW_EVENT = 0x80,
    
    KEY_EVENTS = KEY_PRESS & KEY_RELEASE & KEY_HOLD,
    MOUSE_EVENTS = MOUSE_PRESS & MOUSE_RELEASE & MOUSE_MOVE & MOUSE_SCROLL,
    ALL_EVENTS = KEY_EVENTS & MOUSE_EVENTS & WINDOW_EVENT // more explicit than 0xff
};

#include <functional> // Include for std::function
#include <iostream>   // Include for std::cout (optional, for demonstration)

struct EventListener {

    const event_type_mask mask;
    static void test_fn(KeyPressEvent& ref) { std::cout << " on_keyPress WARNING: noimpl\n"; }

    std::function<void(KeyPressEvent&)> on_keyPress;
    std::function<void(KeyHoldEvent&)> on_keyHold;
    std::function<void(KeyReleaseEvent&)> on_keyRelease;

    std::function<void(MouseClickEvent&)> on_mousePress;
    std::function<void(MouseReleaseEvent&)> on_mouseRelease;
    std::function<void(MouseMoveEvent&)> on_mouseMove;
    std::function<void(MouseScrollEvent&)> on_mouseScroll;

    std::function<void(WindowEvent&)> on_windowEvent;

    EventListener(entt::dispatcher& dispatcher, event_type_mask mask): mask(mask)
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



