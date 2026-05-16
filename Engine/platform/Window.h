/// **************************************** QUAKASTER ENGINE **************************************** 
/// core/Window.h
/// Purpose: Defines a basic Window wrapper for GLFW calls.
/// ************************************************************************************************** 

#pragma once
#include "Resource.h"
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
		HWindow m_hWindow;

		std::string m_Title;

		EventQueue* m_pQueue = nullptr;

		
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
			void makeContextCurrent();

			// Sets the event queue pointer
			void setEventQueue(EventQueue* queue);

			// Gets a pointer to the event queue
			EventQueue* queue();

			// Gets the position of the window
			Window::Position getPosition();

			// Sets the position of the window
			void setPosition(Window::Position new_position);

			// Gets the title of the Window
			std::string getTitle();

			// Returns whether or not this window wants to close
			int shouldClose();
			// Initializes the window
			void init(Window::Size initial_size, const std::string& title, HMonitor monitor, Window* shared = nullptr);
			// Presents the rendered frame and acquires the next swapchain buffer for rendering.
			void swapBuffers();

			// Resizes the window
			void resize(Window::Size new_size);

			Size getSize() const;

			// Gets a platform Resource (GLFWWindow*, SDLWindow*...)
			HWindowRef handle();

			// Explicit destruction
			void destroy();
			Window();
			~Window();


			// Polls events and pushes them to the event queue
			void pollEvents();


	};

	gfx::HLoader getProcAddress();
}