#pragma once
#include <sstream>
#include <SDL.h>
#include <include/GameContext/UI/console_message.h>

struct SDL_Utils {
public:
	// Converts a key code to its representation
	static const char* repr(SDL_Keycode keycode) {
		static const char* unknown = "[UNKNOWN]";
		const char* keyName = SDL_GetKeyName(keycode);
		return keyName ? keyName : unknown;
	}
};

struct KeyPressEvent {
	SDL_Keycode code;
	Uint16 modifier;
	static console_message get_console_message(KeyPressEvent& evt)
	{
		std::stringstream ss;
		ss << "Recieved KeyPressEvent with key " << SDL_Utils::repr(evt.code);
		return { ss.str(), console_color::DEFAULT_TEXT };
	}
};

struct KeyHoldEvent {
	SDL_Keycode code;
	Uint16 modifier;
	static console_message get_console_message(KeyHoldEvent& evt)
	{
		std::stringstream ss;
		ss << "Recieved KeyHoldEvent with key " << SDL_Utils::repr(evt.code);
		return { ss.str(), console_color::DEFAULT_TEXT };
	}
};

struct KeyReleaseEvent {
	SDL_Keycode code;
	Uint16 modifier;
	static console_message get_console_message(KeyReleaseEvent& evt)
	{
		std::stringstream ss;
		ss << "Recieved KeyPressEvent with key " << SDL_Utils::repr(evt.code);
		return { ss.str(), console_color::DEFAULT_TEXT };
	}
};

//uses SDL2 button codes
struct MouseClickEvent {
	Uint8 button; // The mouse button that was clicked
	int x, y;     // Mouse position at the time of click

	static console_message get_console_message(const MouseClickEvent& evt) {
		std::stringstream ss;

		// Translate common mouse buttons to text
		switch (evt.button) {
		case SDL_BUTTON_LEFT:
			ss << "LeftMouseButton";
			break;
		case SDL_BUTTON_RIGHT:
			ss << "RightMouseButton";
			break;
		case SDL_BUTTON_MIDDLE:
			ss << "MiddleMouseButton";
			break;
		default:
			ss << "MouseButton" << static_cast<int>(evt.button);
			break;
		}

		// Add the position to the message
		ss << "click at position (" << evt.x << ", " << evt.y << ")";

		return { ss.str(), console_color::DEFAULT_TEXT };
	}
};

struct MouseHoldEvent {
	Uint8 button; // The mouse button that was clicked
	int x, y;     // Mouse position at the time of click

	static console_message get_console_message(const MouseHoldEvent& evt) {
		std::stringstream ss;

		// Translate common mouse buttons to text
		switch (evt.button) {
		case SDL_BUTTON_LEFT:
			ss << "LeftMouseButton";
			break;
		case SDL_BUTTON_RIGHT:
			ss << "RightMouseButton";
			break;
		case SDL_BUTTON_MIDDLE:
			ss << "MiddleMouseButton";
			break;
		default:
			ss << "MouseButton" << static_cast<int>(evt.button);
			break;
		}

		// Add the position to the message
		ss << "held at position (" << evt.x << ", " << evt.y << ")";

		return { ss.str(), console_color::DEFAULT_TEXT };
	}
};

struct MouseReleaseEvent {
	Uint8 button; // The mouse button that was clicked
	int x, y;     // Mouse position at the time of click

	static console_message get_console_message(const MouseReleaseEvent& evt) {
		std::stringstream ss;

		// Translate common mouse buttons to text
		switch (evt.button) {
		case SDL_BUTTON_LEFT:
			ss << "LeftMouseButton";
			break;
		case SDL_BUTTON_RIGHT:
			ss << "RightMouseButton";
			break;
		case SDL_BUTTON_MIDDLE:
			ss << "MiddleMouseButton";
			break;
		default:
			ss << "MouseButton" << static_cast<int>(evt.button);
			break;
		}

		// Add the position to the message
		ss << "released at position (" << evt.x << ", " << evt.y << ")";

		return { ss.str(), console_color::DEFAULT_TEXT };
	}
};

struct MouseMoveEvent {
	int x, y;     // Current mouse position
	int deltaX, deltaY; // Change in position since last event
	static console_message get_console_message(const MouseMoveEvent& evt) {
		std::stringstream ss;

		ss << "Mouse moved by (" << evt.deltaX << ", " << evt.deltaY << ")";

		return { ss.str(), console_color::DEFAULT_TEXT };
	}
};