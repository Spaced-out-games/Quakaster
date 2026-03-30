/// **************************************** QUAKASTER ENGINE **************************************** 
/// core/Window.h
/// Purpose: Impliments a basic Window wrapper for GLFW calls.
/// ************************************************************************************************** 
#include "Window.h"
#include <GLFW/glfw3.h>
#include "../platform/glfw_EventTranslator.h"
#include "spdlog/spdlog.h"
#include <stdexcept>
namespace qk::core {


	void Window::set_event_queue(EventQueue* queue) {
		m_Queue = queue;
	}
	

	EventQueue* Window::queue() {
		return m_Queue;
	}

	void Window::make_context_current() {
		if (!m_Window) {
			throw std::runtime_error("Window not initialized!");
		}
		glfwMakeContextCurrent(m_Window);
	}

	Window::Position Window::get_position() {
		Position result;
		glfwGetWindowPos(m_Window, &(result.x), &(result.y));
		return result;
	}

	void Window::set_position(Window::Position new_position) {
		
		glfwSetWindowPos(m_Window, new_position.x, new_position.y);
	}

	std::string Window::get_title() {
		return std::string(glfwGetWindowTitle(m_Window));
	}

	void Window::pollEvents() {
		glfwPollEvents();
	}

	int Window::should_close() {
		return glfwWindowShouldClose(m_Window);
	}

	void Window::swap_buffers() {
		glfwSwapBuffers(m_Window);
	}
	void Window::resize(Window::Size new_size) {
		glfwSetWindowSize(m_Window, new_size.w, new_size.h);
	}

	Window::Size Window::get_size() const {
		Window::Size sz;
		glfwGetWindowSize(m_Window, &sz.w, &sz.h);
		return sz;
	}

	void* Window::handle() {
		return m_Window;
	}




	void Window::init(Window::Size initial_size, const std::string& title, Monitor monitor, Window* shared) {
		spdlog::info("Window initialized at {:#x}", (uintptr_t)this);

		m_Window = glfwCreateWindow(initial_size.w, initial_size.h, title.c_str(),
			(GLFWmonitor*)monitor,
			shared ? shared->m_Window : nullptr);

		if (!m_Window) {
			// handle error
			return;
		}

		// Now m_Window is valid — safe to set user pointer and callbacks
		glfwSetWindowUserPointer(m_Window, this);

		// Window events
		glfwSetWindowSizeCallback(m_Window, qk::io::on_window_resize);
		glfwSetFramebufferSizeCallback(m_Window, qk::io::on_framebuffer_resize);
		glfwSetWindowCloseCallback(m_Window, qk::io::on_window_close);
		glfwSetWindowRefreshCallback(m_Window, qk::io::on_window_refresh);
		glfwSetWindowFocusCallback(m_Window, qk::io::on_window_focus);
		glfwSetWindowIconifyCallback(m_Window, qk::io::on_window_minimize);
		glfwSetWindowMaximizeCallback(m_Window, qk::io::on_window_maximize);
		glfwSetWindowContentScaleCallback(m_Window, qk::io::on_window_DPI_update);

		// Input events
		glfwSetKeyCallback(m_Window, qk::io::on_key);
		glfwSetCharCallback(m_Window, qk::io::on_textinput);
		glfwSetCharModsCallback(m_Window, qk::io::on_textinputEx);

		glfwSetMouseButtonCallback(m_Window, qk::io::on_mouse_button);
		glfwSetCursorPosCallback(m_Window, qk::io::on_mouse_move);
		glfwSetCursorEnterCallback(m_Window, qk::io::on_mouse_enter);
		glfwSetScrollCallback(m_Window, qk::io::on_mouse_scroll);

		// Drag & drop
		glfwSetDropCallback(m_Window, qk::io::on_drag_drop);

		// Joystick connection
		glfwSetJoystickCallback(qk::io::on_joystick_connect);
	}

	Window::Window() {

	}

	void Window::destroy() {
		if (m_Window) {
			spdlog::info("Window destroyed at {:#x}", (uintptr_t)this);
			glfwDestroyWindow(m_Window);
			m_Window = nullptr;
		}
	}


	Window::~Window() {
		destroy();
	}

}
#ifdef _WIN32

#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>

namespace qk::core {
	HWND Window::native() {
		return glfwGetWin32Window(m_Window);
	}
}

#endif
