/// **************************************** QUAKASTER ENGINE **************************************** 
/// core/Window.h
/// Purpose: Defines a basic Window wrapper for GLFW calls.
/// ************************************************************************************************** 

#pragma once
#include "../../core.h"
#define GLFW_INCLUDE_NONE
#include <string>
#include <memory>
#include "../io/EventQueue.h"

#ifdef _WIN32
	struct HWND__;
	using HWND = HWND__*;
#endif

struct GLFWwindow;
struct GLFWmonitor;
typedef void (*GLFWkeyfun)(GLFWwindow* window, int key, int scancode, int action, int mods);

using namespace qk::io;


namespace qk::core {

	using Monitor = GLFWmonitor*;


	

	class QK_API Window {
		GLFWwindow* m_Window = nullptr;

		// Pointer to a queue that receives events
		EventQueue* m_Queue = nullptr;

		
		public:

			struct Position {
				int x = 0;
				int y = 0;
			};
			struct Size {
				int w = 0;
				int h = 0;
			};

			// Tells the backend to make this thread current
			void make_context_current();

			// Sets the event queue pointer
			void set_event_queue(EventQueue* queue);

			// Gets a pointer to the event queue
			EventQueue* queue();

			// Gets the position of the window
			Window::Position get_position();

			// Sets the position of the window
			void set_position(Window::Position new_position);

			// Gets the title of the Window
			std::string get_title();

			// Returns whether or not this window wants to close
			int should_close();
			// Initializes the window
			void init(Window::Size initial_size, const std::string& title, Monitor monitor = nullptr, Window* shared = nullptr);
			// Presents the rendered frame and acquires the next swapchain buffer for rendering.
			void swap_buffers();

			// Resizes the window
			void resize(Window::Size new_size);

			Size get_size() const;

			// Gets a platform handle (GLFWWindow*, SDLWindow*...)
			void* handle();
			#ifdef _WIN32
				// Gets a Windows-native handle for this window
				HWND native();
			#else
			#endif
			// Explicit destruction
			void destroy();
			Window();
			~Window();


			// Polls events and pushes them to the event queue
			void pollEvents();

	};
}