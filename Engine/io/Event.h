/// **************************************** QUAKASTER ENGINE **************************************** 
/// core/io/Event.h
/// Purpose: Defines an interface for Quakaster events
/// ************************************************************************************************** 

#pragma once
#include <stdint.h>
#include "core.h"
#include <cstddef>
#include <type_traits>
#include <cassert>
#define QK_MAX_STRING_SIZE 256
#define QK_MAX_PATHS 128
namespace qk {




	// To-do: translate to C-style bitfeild with union with u32 for masks.
	enum class EEventCategory : u32 {
		cat_null		= 0 << 0,
		cat_os			= 1 << 0,
		cat_file		= 1 << 1,
		cat_memory		= 1 << 2,
		cat_engine		= 1 << 3,
		cat_application = 1 << 4,
		cat_window		= 1 << 5,
		cat_graphics	= 1 << 6,
		cat_sound		= 1 << 7,
		cat_physics		= 1 << 8,
		cat_network		= 1 << 9,
		cat_keyboard	= 1 << 10,
		cat_mouse		= 1 << 11,
		cat_continuous  = 1 << 12,
		cat_error		= 1 << 13,
		cat_text		= 1 << 14,
		cat_custom		= 1 << 30,

		cat_kbmouse = cat_mouse | cat_keyboard,

		cat_input = cat_kbmouse, // for now!
		cat_foundational = cat_os | cat_file | cat_memory,
		cat_media = cat_graphics | cat_sound,
		cat_core = cat_engine | cat_application | cat_window,

		cat_all = 0xFFFFFFFF
	};

	
	inline EEventCategory operator|(EEventCategory a, EEventCategory b) {
		return (EEventCategory)((u32)(a) | (u32)(b));
	}

	inline EEventCategory operator&(EEventCategory a, EEventCategory b) {
		return (EEventCategory)((u32)(a) & (u32)(b));
	}

	inline bool operator==(EEventCategory a, EEventCategory b) {
		return (u32)(a) == (u32)(b);
	}

	inline bool operator!=(EEventCategory a, EEventCategory b) {
		return (u32)(a) != (u32)(b);
	}
	inline bool operator!(EEventCategory cat) {
		return cat == EEventCategory::cat_null;
	}

	inline EEventCategory& operator|=(EEventCategory& a, EEventCategory b) {
		a = a | b;
		return a;
	}

	inline EEventCategory& operator&=(EEventCategory& a, EEventCategory b) {
		a = a & b;
		return a;
	}

	inline EEventCategory operator~(EEventCategory a) {
		return (EEventCategory)(~(u32)(a));
	}

	
	enum class EEventType : u32 {
		Null_evt, // Primarily for default construction
		KeyPress_evt,
		KeyHold_evt,
		KeyRelease_evt,

		MousePress_evt,
		MouseRelease_evt,
		MouseHold_evt, 
		MouseMove_evt,
		MouseEnter_evt,
		MouseExit_evt,
		MouseScroll_evt,

		WindowResize_evt,
		WindowGainFocus_evt,
		WindowLoseFocus_evt,
		WindowOpen_evt,
		WindowClose_evt, // needs print support.
		WindowRefresh_evt,
		WindowMinimize_evt,
		WindowMinimizeRestored_evt,
		WindowMaximize_evt,
		WindowMaximizeRestored_evt,

		FramebufferResize_evt,
		WindowDPIUpdate_evt,
		TextInput_evt,
		TextInputEx_evt,
		DragDrop_evt,
		JoystickConnect_evt,
		JoystickDisconnect_evt,
		MonitorConnect_evt,
		MonitorDisconnect_evt,

		PlatformUninitialized_err,
		PlatformNoContext_err,
		PlatformInvalidValue_err,
		PlatformOutOfMemory_err,
		PlatformInterfaceUnavailable_err,
		PlatformVersionUnavailable_err,
		PlatformDriver_err,
		PlatformFormat_err,
		PlatformMouseCursorUnavailable_err



	};

	// The u32 where custom events begin being defined.
	static constexpr u32 CUSTOM_EVENT_BEGIN = 1u << 31;

	

	struct Event {
		// Denotes the type ID that begins the first custom event
		// If you create your own custom events and an enum on top
		//static constexpr u32 custom_begin = 1u << 31;
		Event() = default;
		~Event();

		EEventCategory m_Category = {};
		EEventType m_Type = {};
		u64 timestamp = 0;
		// starts at 128 bit boundary
		alignas(std::max_align_t) u8 m_Payload[16]{};

		template<typename T>
		inline T& view(size_t offset = 0)
		{
			static_assert(std::is_trivially_copyable_v<T>,
				"Event::view<T>: T must be trivially copyable");

			static_assert(std::is_standard_layout_v<T>,
				"Event::view<T>: T must have standard layout");

			constexpr size_t payload_size = sizeof(m_Payload);

			// bounds check
			assert(offset + sizeof(T) <= payload_size &&
				"Event::view<T>: payload overflow");

			// alignment check
			uintptr_t addr = reinterpret_cast<uintptr_t>(m_Payload + offset);
			assert(addr % alignof(T) == 0 &&
				"Event::view<T>: misaligned access");

			return *reinterpret_cast<T*>(m_Payload + offset);
		}

		template<typename T>
		inline const T& view(size_t offset = 0) const
		{
			static_assert(std::is_trivially_copyable_v<T>,
				"Event::view<T>: T must be trivially copyable");

			static_assert(std::is_standard_layout_v<T>,
				"Event::view<T>: T must have standard layout");

			constexpr size_t payload_size = sizeof(m_Payload);

			// bounds check
			assert(offset + sizeof(T) <= payload_size &&
				"Event::view<T>: payload overflow");

			// alignment check
			uintptr_t addr = reinterpret_cast<uintptr_t>(m_Payload + offset);
			assert(addr % alignof(T) == 0 &&
				"Event::view<T>: misaligned access");

			return *reinterpret_cast<const T*>(m_Payload + offset);
		}


	};




	

	// Custom event destruction function pointer type
	using EventEx_dtor_pfn_t = void(*)(Event&);

	// Sets the event destruction callback function
	void QK_API SetEventExDtor_fn(EventEx_dtor_pfn_t dtor);




}