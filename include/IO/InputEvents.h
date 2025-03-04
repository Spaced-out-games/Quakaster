#pragma once
#include <sstream>
#include <SDL.h>
#include <include/UI/console_message.h>

struct SDL_Utils {
public:
	// Converts a key code to its representation
	static const char* repr(SDL_Keycode keycode) {
		static const char* unknown = "[UNKNOWN]";
		const char* keyName = SDL_GetKeyName(keycode);
		return keyName ? keyName : unknown;
	}
};

// Triggered when a key is pressed
struct KeyPressEvent { SDL_Keycode code; Uint16 mod; KeyPressEvent(SDL_Keycode code, Uint16 mod = KMOD_NONE) : code(code), mod(mod) {} };

// Triggered when a key is held down
struct KeyHoldEvent { SDL_Keycode code; Uint32 duration; KeyHoldEvent(SDL_Keycode code) : code(code), duration(duration) {} };

// Triggered when a key is released
struct KeyReleaseEvent { SDL_Keycode code; Uint16 mod; KeyReleaseEvent(SDL_Keycode code, Uint16 mod = KMOD_NONE) : code(code), mod(mod) {} };

// Triggered when a mouse button is pressed
struct MouseClickEvent {
	Uint8 button;
	bool down;
	Sint32 x;
	Sint32 y;
	MouseClickEvent(Uint8 button, Sint32 x, Sint32 y,bool down) : button(button), down(down), x(x), y(y) {}
};

// Triggered when a mouse button is released
struct MouseReleaseEvent { Uint8 button; Uint16 mod; MouseReleaseEvent(Uint8 button, Uint16 mod = KMOD_NONE) : button(button), mod(mod) {} };

// Triggered when the mouse is moved
struct MouseMoveEvent {
	glm::ivec2 current_position;
	glm::ivec2 relative_position;
	Uint16 mod;
	MouseMoveEvent(glm::ivec2 current, glm::ivec2 delta) : current_position(current), relative_position(delta){
	}
};

// Triggered when the mouse wheel is scrolled
struct MouseScrollEvent { int x, y; Uint16 mod; MouseScrollEvent(int x, int y, Uint16 mod = KMOD_NONE) : x(x), y(y), mod(mod) {} };

// Triggered when the window state changes
struct WindowEvent {
	Uint8 event_type;   // Type of window event (e.g., SDL_WINDOWEVENT_RESIZED, SDL_WINDOWEVENT_FOCUS_GAINED, etc.)
	int data1;          // Data associated with the event (e.g., width for resize)
	int data2;          // Additional data (e.g., height for resize)

	WindowEvent(Uint8 event_type, int data1 = 0, int data2 = 0)
		: event_type(event_type), data1(data1), data2(data2) {}
};

struct PauseEvent {
	bool new_pause;
};