#pragma once
#include <SDL.h>
#include <functional>
#include "scene.h"
#include "entity.h"

#define ADD_CONTROLLER_LISTENERS \
void add_event_listeners() \
    { \
        ADD_EVENT_LISTENER(app.scene.dispatcher, KeyPressEvent, on_KeyPress); \
        ADD_EVENT_LISTENER(app.scene.dispatcher, KeyHoldEvent, on_KeyHeld); \
        ADD_EVENT_LISTENER(app.scene.dispatcher, KeyReleaseEvent, on_KeyRelease); \
        ADD_EVENT_LISTENER(app.scene.dispatcher, MouseClickEvent, on_MouseClick); \
        ADD_EVENT_LISTENER(app.scene.dispatcher, MouseHoldEvent, on_MouseClickHold); \
        ADD_EVENT_LISTENER(app.scene.dispatcher, MouseReleaseEvent, on_MouseClickRelease); \
        ADD_EVENT_LISTENER(app.scene.dispatcher, MouseButtonClickEvent, on_MouseButtonClick); \
    } \

// Converts a key code to its representation
class SDL_Utils {
public:
    // Converts a key code to its representation
    static const char* repr(SDL_Keycode keycode) {
        static const char* unknown = "[UNKNOWN]";
        const char* keyName = SDL_GetKeyName(keycode);
        return keyName ? keyName : unknown;
    }
};

// Event structures
struct KeyPressEvent {

    char key;    // The key that was pressed
    Uint16 mod;  // Modifier keys (e.g., Shift, Ctrl)

    static void print(KeyPressEvent event) {
        console_log(std::string("Received KeyPressEvent with key ") + SDL_Utils::repr(event.key), console_colors::RED);
    }
};

struct KeyHoldEvent {
    SDL_Keycode key; // The key being held down
    Uint16 mod;      // Modifier keys
};

struct KeyReleaseEvent {
    SDL_Keycode key; // The key that was released
    Uint16 mod;      // Modifier keys
};

struct MouseClickEvent {
    Uint8 button; // The mouse button that was clicked
    int x, y;     // Mouse position at the time of click
};

struct MouseHoldEvent {
    Uint8 button; // The mouse button being held down
    int x, y;     // Mouse position
};

struct MouseReleaseEvent {
    Uint8 button; // The mouse button that was released
    int x, y;     // Mouse position at the time of release
};

struct MouseButtonClickEvent {
    Uint8 button; // The mouse button that was clicked
    int x, y;     // Mouse position at the time of click
    Uint32 clickCount; // Number of clicks
};

struct MouseMoveEvent {
    int x, y;     // Current mouse position
    int deltaX, deltaY; // Change in position since last event
};

// The Controller class handles input events from SDL and dispatches them
class Controller {
public:
    Application& app; // Reference to the application instance

    // Tracks the last mouse position for movement events
    int lastMouseX = 0;
    int lastMouseY = 0;

    // Derived classes must register their own implementations of event handlers
    // ADD_CONTROLLER_LISTENERS macro can be used for this purpose

    // Event handler functions (to be overridden in derived classes)
    static void on_KeyPress(KeyPressEvent evt) {};
    static void on_KeyHeld(KeyHoldEvent evt) {};
    static void on_KeyRelease(KeyReleaseEvent evt) {};
    static void on_MouseClick(MouseClickEvent evt) {};
    static void on_MouseClickHold(MouseHoldEvent evt) {};
    static void on_MouseClickRelease(MouseReleaseEvent evt) {};
    static void on_MouseButtonClick(MouseButtonClickEvent evt) {};

    // Constructor: Initializes the Controller with the given application instance
    explicit Controller(Application& app)
        : app(app), lastMouseX(0), lastMouseY(0) {}

    // Processes input events from SDL
    void dispatchIOEvents() {
        SDL_Event event;

        while (SDL_PollEvent(&event)) {
            // Toggle console when Escape is pressed
            if (event.key.keysym.sym == SDLK_ESCAPE && event.type == SDL_KEYDOWN) {
                Application::current_application->console.open ^= 1;
            }

            // Step 1: Send event to ImGui if console is open
            if (app.console.open) {
                ImGui_ImplSDL2_ProcessEvent(&event);
                // Delegate to ImGui only if it's handling the input
                if (ImGui::GetIO().WantCaptureKeyboard || ImGui::GetIO().WantCaptureMouse) {
                    continue; // Skip processing this event in the Controller
                }
            }

            // Step 2: Process event using the custom event system
            switch (event.type) {
            case SDL_KEYDOWN:
                if (!event.key.repeat) {
                    KeyPressEvent keyPressEvent{ event.key.keysym.sym, event.key.keysym.mod };
                    app.scene.dispatcher.trigger(keyPressEvent);
                }
                else {
                    KeyHoldEvent keyHoldEvent{ event.key.keysym.sym, event.key.keysym.mod };
                    app.scene.dispatcher.trigger(keyHoldEvent);
                }
                break;

            case SDL_KEYUP: {
                KeyReleaseEvent keyReleaseEvent{ event.key.keysym.sym, event.key.keysym.mod };
                app.scene.dispatcher.trigger(keyReleaseEvent);
                break;
            }

            case SDL_MOUSEBUTTONDOWN: {
                MouseClickEvent mouseClickEvent{ event.button.button, event.button.x, event.button.y };
                app.scene.dispatcher.trigger(mouseClickEvent);
            } break;

            case SDL_MOUSEBUTTONUP: {
                MouseReleaseEvent mouseReleaseEvent{ event.button.button, event.button.x, event.button.y };
                app.scene.dispatcher.trigger(mouseReleaseEvent);

                if (event.button.clicks > 1) {
                    MouseButtonClickEvent mouseButtonClickEvent{
                        event.button.button,
                        event.button.x,
                        event.button.y,
                        event.button.clicks };
                    app.scene.dispatcher.trigger(mouseButtonClickEvent);
                }
            } break;

            case SDL_MOUSEMOTION: {
                int deltaX = event.motion.x - lastMouseX;
                int deltaY = event.motion.y - lastMouseY;

                MouseMoveEvent mouseMoveEvent{
                    event.motion.x, event.motion.y, deltaX, deltaY };
                app.scene.dispatcher.trigger(mouseMoveEvent);

                // Update last mouse position
                lastMouseX = event.motion.x;
                lastMouseY = event.motion.y;
            } break;

            case SDL_QUIT:
                // Handle quit event (e.g., exit the application loop)
                exit(0);
                break;

            default:
                break;
            }
        }
    }
};

// The basic_controller class is a concrete implementation of the Controller class
class basic_controller : public Controller {
public:

    Entity& target;
    basic_controller(Application& app, Entity& pawn) : Controller(app), target(pawn) { add_event_listeners(); }

    // Register event listeners for input events
    ADD_CONTROLLER_LISTENERS

        // Override the event handler functions to provide specific behavior
        static void on_KeyPress(KeyPressEvent event) {

        console_log(SDL_Utils::repr(event.key));
    }

    static void on_KeyHeld(KeyHoldEvent event) {
        console_log(SDL_Utils::repr(event.key));
    }

    static void on_KeyRelease(KeyReleaseEvent event) {
        // Handle key release event
    }

    static void on_MouseClick(MouseClickEvent event) {
        // Handle mouse click event
    }

    static void on_MouseClickHold(MouseHoldEvent event) {
        // Handle mouse click hold event
    }

    static void on_MouseClickRelease(MouseReleaseEvent event) {
        // Handle mouse release event
    }

    static void on_MouseButtonClick(MouseButtonClickEvent event) {
        // Handle mouse button click event
    }
};