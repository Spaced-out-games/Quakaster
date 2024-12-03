#include <SDL2/SDL.h>
#include <functional>
#include "scene.h"

struct KeyPressEvent {
    SDL_Keycode key;
    Uint16 mod;
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
    entt::dispatcher& dispatcher;

    // Tracks the last mouse position
    int lastMouseX = 0;
    int lastMouseY = 0;

    explicit Controller(entt::dispatcher& dispatcher)
        : dispatcher(dispatcher), lastMouseX(0), lastMouseY(0) {}

    void processInput() {
        SDL_Event event;

        while (SDL_PollEvent(&event)) {
            switch (event.type) {
            case SDL_KEYDOWN:
                if (!event.key.repeat) {
                    KeyPressEvent keyPressEvent{ event.key.keysym.sym, event.key.keysym.mod };
                    dispatcher.trigger(keyPressEvent);
                }
                else {
                    KeyHoldEvent keyHoldEvent{ event.key.keysym.sym, event.key.keysym.mod };
                    dispatcher.trigger(keyHoldEvent);
                }
                break;

            case SDL_KEYUP:
                KeyReleaseEvent keyReleaseEvent{ event.key.keysym.sym, event.key.keysym.mod };
                dispatcher.trigger(keyReleaseEvent);
                break;

            case SDL_MOUSEBUTTONDOWN: {
                MouseClickEvent mouseClickEvent{ event.button.button, event.button.x, event.button.y };
                dispatcher.trigger(mouseClickEvent);
            } break;

            case SDL_MOUSEBUTTONUP: {
                MouseReleaseEvent mouseReleaseEvent{ event.button.button, event.button.x, event.button.y };
                dispatcher.trigger(mouseReleaseEvent);

                if (event.button.clicks > 1) {
                    MouseButtonClickEvent mouseButtonClickEvent{
                        event.button.button,
                        event.button.x,
                        event.button.y,
                        event.button.clicks };
                    dispatcher.trigger(mouseButtonClickEvent);
                }
            } break;

            case SDL_MOUSEMOTION: {
                int deltaX = event.motion.x - lastMouseX;
                int deltaY = event.motion.y - lastMouseY;

                MouseMoveEvent mouseMoveEvent{
                    event.motion.x, event.motion.y, deltaX, deltaY };
                dispatcher.trigger(mouseMoveEvent);

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
