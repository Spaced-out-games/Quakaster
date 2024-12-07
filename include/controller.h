#include <SDL.h>
#include <functional>
#include "scene.h"

class SDLKeyConverter {
public:
    // Convert SDL_Keycode to a representative string
    static std::string to_string(SDL_Keycode keycode) {
        // Handle alphanumeric and special keys
        switch (keycode) {
        case SDLK_a: case SDLK_b: case SDLK_c: case SDLK_d: case SDLK_e:
        case SDLK_f: case SDLK_g: case SDLK_h: case SDLK_i: case SDLK_j:
        case SDLK_k: case SDLK_l: case SDLK_m: case SDLK_n: case SDLK_o:
        case SDLK_p: case SDLK_q: case SDLK_r: case SDLK_s: case SDLK_t:
        case SDLK_u: case SDLK_v: case SDLK_w: case SDLK_x: case SDLK_y:
        case SDLK_z:
            return std::string(1, static_cast<char>(keycode));
        case SDLK_0: case SDLK_1: case SDLK_2: case SDLK_3: case SDLK_4:
        case SDLK_5: case SDLK_6: case SDLK_7: case SDLK_8: case SDLK_9:
            return std::string(1, static_cast<char>(keycode));
        case SDLK_SPACE: return " ";
        case SDLK_RETURN: return "[ENTER]";
        case SDLK_BACKSPACE: return "[BACKSPACE]";
        case SDLK_ESCAPE: return "[ESC]";
        case SDLK_LSHIFT: return "[LSHIFT]";
        case SDLK_RSHIFT: return "[RSHIFT]";
        case SDLK_LCTRL: return "[LCTRL]";
        case SDLK_RCTRL: return "[RCTRL]";
        case SDLK_TAB: return "[TAB]";
            // Add more keys as needed
        default: return "[UNKNOWN]";
        }
    }
};

struct KeyPressEvent {
    char key;
    Uint16 mod;
    static void print(KeyPressEvent event) {
        console_log(std::string("Recieved KeyPressEvent with key") + SDLKeyConverter::to_string(event.key), console_colors::RED);
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
        ADD_EVENT_LISTENER(app.scene.dispatcher, KeyPressEvent, default_move);
    }

    static void default_move(KeyPressEvent event) {

        switch (event.key)
        {
        case(SDLK_w):
            //console_log("w key event");
            //break;


        default:
            #ifdef _DEBUG
                std::stringstream ss;
                ss << "Key '" << SDLKeyConverter::to_string(event.key) << "' pressed";
                console_log(ss.str());
                #endif // _DEBUG
            break;
        }


        if (event.key == SDLK_w)
        {
            console_log("w key event");

        }
        if (event.key == SDLK_ESCAPE)
        {

        }
    }


    void processInput() {
        SDL_Event event;




        while (SDL_PollEvent(&event)) {

            // Escape key is handled differently. It toggles the console, regardless of whether the console is open
            // if(event.type == SDL_KEYDOWN)
            if (event.key.keysym.sym == SDLK_ESCAPE && event.type == SDL_KEYDOWN)
            {
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
