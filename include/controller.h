#include <SDL.h>
#include <functional>
#include "scene.h"

char SDLKeyToChar(SDL_Keycode keycode, bool shiftPressed = false) {
    // Handle alphabetic keys
    if (keycode >= SDLK_a && keycode <= SDLK_z) {
        return shiftPressed ? (keycode - SDLK_a + 'A') : (keycode - SDLK_a + 'a');
    }
    // Handle numeric keys
    if (keycode >= SDLK_0 && keycode <= SDLK_9) {
        return static_cast<char>(keycode);
    }
    // Handle space
    if (keycode == SDLK_SPACE) {
        return ' ';
    }

    // Add more mappings as needed
    return '\0'; // Return null char for unsupported keys
}

struct KeyPressEvent {
    SDL_Keycode key;
    Uint16 mod;
    static void print(KeyPressEvent event) {
        console_log(std::string("Recieved KeyPressEvent"),console_colors::RED);
        if (event.key == SDLK_ESCAPE)
        {
            Application::current_application->console.open ^= 1;

        }
        //event.target->log(event.color_ID, event.message.c_str());
    }
};

struct KeyHoldEvent {
    SDL_Keycode key;
    Uint16 mod;
};

struct KeyReleaseEvent {
    SDL_Keycode key;
    Uint16 mod;
};

struct MouseClickEvent {
    Uint8 button;
    int x, y;
};

struct MouseHoldEvent {
    Uint8 button;
    int x, y;
};

struct MouseReleaseEvent {
    Uint8 button;
    int x, y;
};

struct MouseButtonClickEvent {
    Uint8 button;
    int x, y;
    Uint32 clickCount;
};

struct MouseMoveEvent {
    int x, y; // Current position
    int deltaX, deltaY; // Delta since last poll
};


class Controller {
public:
    Application& app;

    // Tracks the last mouse position
    int lastMouseX = 0;
    int lastMouseY = 0;

    explicit Controller(Application& app)
        : app(app), lastMouseX(0), lastMouseY(0)
    {
        ADD_EVENT_LISTENER(app.scene.dispatcher, KeyPressEvent, KeyPressEvent::print);
    }

    void processInput() {
        
        SDL_Event event;

        while (SDL_PollEvent(&event)) {

            if (app.console.open) {
                ImGui_ImplSDL2_ProcessEvent(&event);
                return;
            }
            
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
