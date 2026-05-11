/// **************************************** QUAKASTER ENGINE **************************************** 
/// core/io/DefaultEvents.h
/// Purpose: Declares event views for easy Event manipulation
/// ************************************************************************************************** 
#pragma once
#include "Event.h"
#include <string>
#include <format>

namespace qk::io {

	struct NullEvent : Event {};

	struct KeyEvent : Event {
		inline int& key() { return view<int>(0); }
		inline const int& key() const { return view<const int>(0); }

		inline int& scancode() { return view<int>(sizeof(int)); }
		inline const int& scancode() const { return view<const int>(sizeof(int)); }

		inline int& mods() { return view<int>(sizeof(int) * 2); }
		inline const int& mods() const { return view<const int>(sizeof(int) * 2); }
	};
	// helper wrapper types. 

	struct KeyPressEvent : KeyEvent {};
	struct KeyReleaseEvent : KeyEvent {};
	struct KeyHoldEvent : KeyEvent {};

	struct MouseEvent : Event {
		inline int& button() { return view<int>(0); }
		inline const int& button() const { return view<const int>(0); }

		inline int& mods() { return view<int>(sizeof(int)); }
		inline const int& mods() const { return view<const int>(sizeof(int)); }
	};

	struct MousePressEvent: MouseEvent {};
	struct MouseReleaseEvent : MouseEvent {};
	struct MouseHoldEvent : MouseEvent {};




	struct MouseMoveEvent : Event {
		inline float& x() { return view<float>(0); }
		inline const float& x() const { return view<const float>(0); }

		inline float& y() { return view<float>(sizeof(float)); }
		inline const float& y() const { return view<const float>(sizeof(float)); }
	};

	struct MouseEnterEvent : Event {};

	struct MouseExitEvent : Event {};

	struct MouseScrollEvent : Event {
		inline float& dx() { return view<float>(0); }
		inline const float& dx() const { return view<const float>(0); }

		inline float& dy() { return view<float>(sizeof(float)); }
		inline const float& dy() const { return view<const float>(sizeof(float)); }
	};

	struct WindowResizeEvent : Event {
		inline int& width() { return view<int>(0); }
		inline const int& width() const { return view<const int>(0); }

		inline int& height() { return view<int>(sizeof(int)); }
		inline const int& height() const { return view<const int>(sizeof(int)); }
	};

	struct WindowCloseEvent : Event {};

	struct WindowRefreshEvent: Event {};

	struct WindowGainFocusEvent : Event {};

	struct WindowLoseFocusEvent : Event {};

	struct WindowMinimizeEvent : Event {};

	struct WindowMinimizeRestoreEvent : Event {};

	struct WindowMaximizeEvent: Event {};

	struct WindowMaximizeRestoreEvent : Event {};

	struct WindowFramebufferResizeEvent : Event {
		inline int& width() { return view<int>(0); }
		inline const int& width() const { return view<const int>(0); }

		inline int& height() { return view<int>(sizeof(int)); }
		inline const int& height() const { return view<const int>(sizeof(int)); }
	};

	struct WindowDPIUpdateEvent : Event {
		inline float& xscale() { return view<float>(0); }
		inline const float& xscale() const { return view<const float>(0); }

		inline float& yscale() { return view<float>(sizeof(float)); }
		inline const float& yscale() const { return view<const float>(sizeof(float)); }
	};

	struct TextInputEvent : Event {
		inline unsigned int& code() { return view<unsigned int>(0); }
		inline const unsigned int& code() const { return view<const unsigned int>(0); }
	};

	struct TextInputEventEx : Event {
		inline int& code() { return view<int>(0); }
		inline const int& code() const { return view<const int>(0); }

		inline int& mod() { return view<int>(sizeof(int)); }
		inline const int& mod() const { return view<const int>(sizeof(int)); }
	};

	struct DragDropEvent : Event {
		inline int& count() { return view<int>(0); }
		char*& paths() { return view<char*>(sizeof(int)); }
		char* paths(size_t i) { return paths() + (QK_MAX_STRING_SIZE * i); }
	};

	struct JoystickConnectEvent : Event {
		inline int& jid() { return view<int>(0); }
		inline const int& jid() const { return view<const int>(0); }
	};

	struct JoystickDisconnectEvent : Event {
		inline int& jid() { return view<int>(0); }
		inline const int& jid() const { return view<const int>(0); }
	};

	
	struct MonitorConnectEvent : Event {};

	struct MonitorDisconnectEvent: Event {};

	struct Error : Event {
		inline char*& description() { return view<char*>(0); }
		inline char* const& description() const { return view<char* const>(0); }
	};

	struct PlatformUninitializedError : Error {};
	
	struct PlatformNoContextError : Error {};
	
	struct PlatformOutOfMemoryError: Error {};
	
	struct PlatformInterfaceUnavailableError: Error {};
	
	struct PlatformVersionUnavailableError: Error {};
	
	struct PlatformDriverError: Error {};
	
	struct PlatformFormatError: Error {};
	
	struct PlatformMouseCursorUnavailableError: Error {};

	// Function pointer type to convert custom events to string representations
	using EventExToString_pfn_t = std::string(*)(const Event&);

	// Function pointer to convert custom event to string representation
	inline EventExToString_pfn_t g_EventExToString_pfn = nullptr;

	// Converts any Event to a string
	std::string QK_API to_string(const Event& evt);


}