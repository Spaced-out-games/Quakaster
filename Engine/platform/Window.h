/// **************************************** QUAKASTER ENGINE **************************************** 
/// core/Window.h
/// Purpose: Defines a basic Window wrapper for GLFW calls.
/// ************************************************************************************************** 

#pragma once
#include "Handle.h"
#include "core.h"
#include <string>
#include <memory>
#include "../io/EventQueue.h"
#include "gfx_rhi.h"
#include "SharedDefs.h"

namespace qk {


	class QK_API Window;


	void QK_API InitWindowEventSystem(Window& window);
	

	class QK_API Window {
		HWindow m_Handle;

		std::string m_Title;

		//GLFWwindow* m_Window = nullptr;

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
			void init(Window::Size initial_size, const std::string& title, HMonitor monitor, Window* shared = nullptr);
			// Presents the rendered frame and acquires the next swapchain buffer for rendering.
			void swap_buffers();

			// Resizes the window
			void resize(Window::Size new_size);

			Size get_size() const;

			// Gets a platform handle (GLFWWindow*, SDLWindow*...)
			HWindow handle();

			// Explicit destruction
			void destroy();
			Window();
			~Window();


			// Polls events and pushes them to the event queue
			void pollEvents();


			void* getProcAddress_pfn();


	};
}