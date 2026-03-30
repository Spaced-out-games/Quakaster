/// **************************************** QUAKASTER ENGINE **************************************** 
/// core/platform/glfw_EventTranslator.h
/// Purpose: Implements function callbacks for converting GLFW events to Quakaster 
/// events and loading them into a queue.
/// ************************************************************************************************** 

#include "glfw_EventTranslator.h"
#include "../io/DefaultEvents.h"
#include "../io/Event.h"
#include "../core/Window.h"
#include <algorithm>
#include "../utility/time.h"

using namespace qk;

namespace qk::io {
	void on_key(GLFWwindow* window, int key, int scancode, int action, int mods) {

		using enum EEventCategory;
		using enum EEventType;
		core::Window* target = (core::Window*)glfwGetWindowUserPointer(window);
		auto* queue = target->queue();
		if (!queue) return;

		KeyEvent& kv = (KeyEvent&)queue->emplace_back();
		kv.m_Category = cat_keyboard;
		switch (action) {
		case GLFW_PRESS:  kv.m_Type = KeyPress_evt; break;
		case GLFW_REPEAT: kv.m_Type = KeyHold_evt;  break;
		case GLFW_RELEASE:kv.m_Type = KeyRelease_evt; break;
		}


		kv.timestamp = util::now();
		kv.scancode() = scancode;
		kv.mods() = mods;
		kv.key() = key;

	}

	void QK_API on_mouse_button(GLFWwindow* window, int button, int action, int mods)
	{
		using enum EEventCategory;
		using enum EEventType;

		core::Window* target = (core::Window*)glfwGetWindowUserPointer(window);
		auto* queue = target->queue();
		if (!queue) return;

		MouseEvent& ev = (MouseEvent&)queue->emplace_back();
		ev.m_Category = cat_mouse;

		ev.timestamp = util::now();
		ev.button() = button;
		ev.mods() = mods;

		if (action == GLFW_PRESS)
			ev.m_Type = MousePress_evt;
		else if (action == GLFW_RELEASE)
			ev.m_Type = MouseRelease_evt;
		else
			ev.m_Type = MouseHold_evt;
	}

	void on_mouse_move(GLFWwindow* window, double xpos, double ypos) {

		using enum EEventCategory;
		using enum EEventType;
		core::Window* target = (core::Window*)glfwGetWindowUserPointer(window);
		auto* queue = target->queue();
		if (!queue) return;

		MouseMoveEvent& mv = (MouseMoveEvent&)queue->emplace_back();
		mv.timestamp = util::now();
		mv.m_Category = cat_continuous | cat_mouse;
		mv.m_Type = MouseMove_evt;
		mv.x() = (float)xpos;
		mv.y() = (float)ypos;

	}

	void on_mouse_enter(GLFWwindow* window, int entered) {

		using enum EEventCategory;
		using enum EEventType;
		core::Window* target = (core::Window*)glfwGetWindowUserPointer(window);
		auto* queue = target->queue();
		if (!queue) return;

		Event& mv = queue->emplace_back();
		mv.m_Category = cat_mouse | cat_window;
		mv.timestamp = util::now();

		if (entered) {
			mv.m_Type = MouseEnter_evt;
		}
		else {
			mv.m_Type = MouseExit_evt;
		}
	}

	void on_mouse_scroll(GLFWwindow* window, double xoffset, double yoffset) {
		using enum EEventCategory;
		using enum EEventType;
		core::Window* target = (core::Window*)glfwGetWindowUserPointer(window);
		auto* queue = target->queue();
		if (!queue) return;
		MouseScrollEvent& sv = (MouseScrollEvent&)queue->emplace_back();
		sv.timestamp = util::now();

		sv.m_Category = cat_mouse | cat_continuous;
		sv.m_Type = MouseScroll_evt;
		sv.dx() = (float)xoffset;
		sv.dy() = (float)yoffset;

	}

	void on_window_resize(GLFWwindow* window, int width, int height) {
		using enum EEventCategory;
		using enum EEventType;
		core::Window* target = (core::Window*)glfwGetWindowUserPointer(window);
		auto* queue = target->queue();
		if (!queue) return;
		WindowResizeEvent& rv = (WindowResizeEvent&)queue->emplace_back();
		rv.timestamp = util::now();

		rv.m_Category = cat_window | cat_continuous;
		rv.m_Type = WindowResize_evt;
		rv.height() = height;
		rv.width() = width;

	}

	void on_window_close(GLFWwindow* window) {
		using enum EEventCategory;
		using enum EEventType;
		core::Window* target = (core::Window*)glfwGetWindowUserPointer(window);
		auto* queue = target->queue();
		if (!queue) return;
		WindowCloseEvent& cv = (WindowCloseEvent&)queue->emplace_back();
		cv.timestamp = util::now();

		cv.m_Category = cat_window;
		cv.m_Type = WindowClose_evt;
	}

	void on_window_refresh(GLFWwindow* window) {
		using enum EEventCategory;
		using enum EEventType;
		core::Window* target = (core::Window*)glfwGetWindowUserPointer(window);
		auto* queue = target->queue();
		if (!queue) return;
		WindowRefreshEvent& rv = (WindowRefreshEvent&)queue->emplace_back();
		rv.m_Category = cat_window;
		rv.timestamp = util::now();

		rv.m_Type = WindowRefresh_evt;
	}

	void on_window_focus(GLFWwindow* window, int focus) {
		using enum EEventCategory;
		using enum EEventType;
		core::Window* target = (core::Window*)glfwGetWindowUserPointer(window);
		auto* queue = target->queue();
		if (!queue) return;
		Event& ev = queue->emplace_back();
		ev.timestamp = util::now();

		ev.m_Category = cat_window;
		if (focus) {
			ev.m_Type = WindowGainFocus_evt;
		}
		else {
			ev.m_Type = WindowLoseFocus_evt;
		}
	}

	void on_window_minimize(GLFWwindow* window, int iconified) {
		using enum EEventCategory;
		using enum EEventType;
		core::Window* target = (core::Window*)glfwGetWindowUserPointer(window);
		auto* queue = target->queue();
		if (!queue) return;
		Event& ev = queue->emplace_back();
		ev.timestamp = util::now();

		ev.m_Category = cat_window;

		if (iconified) {
			ev.m_Type = WindowMinimize_evt;
		}
		else {
			ev.m_Type = WindowMinimizeRestored_evt;
		}

	}

	void on_window_maximize(GLFWwindow* window, int maximized) {
		using enum EEventCategory;
		using enum EEventType;
		core::Window* target = (core::Window*)glfwGetWindowUserPointer(window);
		auto* queue = target->queue();

		if (!queue) return;
		Event& ev = queue->emplace_back();
		ev.timestamp = util::now();

		ev.m_Category = cat_window;
		ev.m_Type = maximized ? WindowMaximize_evt : WindowMaximizeRestored_evt;
	}

	void on_framebuffer_resize(GLFWwindow* window, int width, int height) {
		
		// NOTICE: This couples us to OpenGL, but until we have some kind of viewport flushing
		// mechanism going, this will have to work.
		//glViewport(0, 0, width, height);
		using enum EEventCategory;
		using enum EEventType;
		core::Window* target = (core::Window*)glfwGetWindowUserPointer(window);
		auto* queue = target->queue();

		if (!queue) return;
		WindowFramebufferResizeEvent& ev = (WindowFramebufferResizeEvent&)queue->emplace_back();
		ev.timestamp = util::now();

		ev.m_Category = cat_window;
		ev.m_Type = FramebufferResize_evt;

		ev.width() = width;
		ev.height() = height;

	}

	void on_window_DPI_update(GLFWwindow* window, float xscale, float yscale) {
		using enum EEventCategory;
		using enum EEventType;
		core::Window* target = (core::Window*)glfwGetWindowUserPointer(window);
		auto* queue = target->queue();

		if (!queue) return;
		WindowDPIUpdateEvent& ev = (WindowDPIUpdateEvent&)queue->emplace_back();
		ev.timestamp = util::now();
		ev.m_Category = cat_window;
		ev.m_Type = WindowDPIUpdate_evt;
		ev.xscale() = (float)xscale;
		ev.yscale() = (float)yscale;
	}
	void on_textinput(GLFWwindow* window, unsigned int codepoint) {
		using enum EEventCategory;
		using enum EEventType;
		core::Window* target = (core::Window*)glfwGetWindowUserPointer(window);
		auto* queue = target->queue();

		if (!queue) return;
		TextInputEvent& ev = (TextInputEvent&)queue->emplace_back();
		ev.timestamp = util::now();
		ev.m_Category = cat_text;
		ev.m_Type = TextInput_evt;
		ev.code() = codepoint;
	
	}

	void on_textinputEx(GLFWwindow* window, unsigned int codepoint, int modifiers){
		using enum EEventCategory;
		using enum EEventType;
		core::Window* target = (core::Window*)glfwGetWindowUserPointer(window);
		auto* queue = target->queue();

		if (!queue) return;
		TextInputEventEx& ev = (TextInputEventEx&)queue->emplace_back();
		ev.timestamp = util::now();
		ev.m_Category = cat_text;
		ev.m_Type = TextInputEx_evt;
		ev.code() = codepoint;
		ev.mod() = modifiers;
	}
	void on_drag_drop(GLFWwindow* window, int pathcount, const char* paths[])
	{
		pathcount = std::min(pathcount, QK_MAX_PATHS);
		using enum EEventCategory;
		using enum EEventType;

		core::Window* target = (core::Window*)glfwGetWindowUserPointer(window);
		auto* queue = target->queue();
		if (!queue) return;

		DragDropEvent& ev = (DragDropEvent&)queue->emplace_back();
		ev.timestamp = util::now();
		ev.m_Category = cat_window;
		ev.m_Type = DragDrop_evt;
		ev.count() = pathcount;

		char* pool = (char*)malloc(QK_MAX_STRING_SIZE * pathcount);
		if (!pool) return;

		for (int i = 0; i < pathcount; i++)
		{
			char* dst = pool + (QK_MAX_STRING_SIZE * i);
			strcpy_s(dst, QK_MAX_STRING_SIZE, paths[i]);
		}

		ev.paths() = pool;
	}
	void on_joystick_connect(int jid, int event){
		/*
		using enum EEventCategory;
		using enum EEventType;

		Window* target = (Window*)glfwGetWindowUserPointer(window);
		auto* queue = target->queue();
		if (!queue) return;
		JoystickConnectEvent& jc = (JoystickConnectEvent&)queue->emplace_back();

		jc.m_Category = cat_os;
		jc.jid() = jid;
		
		if (event) {
			jc.m_Type = JoystickConnect_evt;
		}
		else {
			jc.m_Type = JoystickDisconnect_evt;
		}
		*/


	}
	void on_monitor_event(GLFWmonitor* monitor, int event)
	{
		/*
		using enum EEventCategory;
		using enum EEventType;

		Window* target = (Window*)glfwGetWindowUserPointer(window);
		auto* queue = target->queue();
		if (!queue) return;

		Event& ev = queue->emplace_back();
		ev.m_Category = cat_os;

		if (event == GLFW_CONNECTED)
		{
			ev.m_Type = MonitorConnect_evt;
		}
		else
		{
			ev.m_Type = MonitorDisconnect_evt;
		}*/
	}
	void on_glfw_error(int code, const char* description)
	{
		/*
		using enum EEventCategory;
		using enum EEventType;
		Window* target = (Window*)glfwGetWindowUserPointer(window);
		auto* queue = target->queue();
		if (!queue) return;

		Error& ev = (Error&)queue->emplace_back();
		ev.m_Category = cat_error;

		switch (code)
		{
		case GLFW_NOT_INITIALIZED:
			ev.m_Type = PlatformUninitialized_err;
			break;

		case GLFW_NO_CURRENT_CONTEXT:
			ev.m_Type = PlatformNoContext_err;
			break;

		case GLFW_INVALID_VALUE:
			ev.m_Type = PlatformInvalidValue_err;
			break;

		case GLFW_OUT_OF_MEMORY:
			ev.m_Type = PlatformOutOfMemory_err;
			break;

		case GLFW_API_UNAVAILABLE:
			ev.m_Type = PlatformInterfaceUnavailable_err;
			break;

		case GLFW_VERSION_UNAVAILABLE:
			ev.m_Type = PlatformVersionUnavailable_err;
			break;

		case GLFW_PLATFORM_ERROR:
			ev.m_Type = PlatformDriver_err;
			break;

		case GLFW_FORMAT_UNAVAILABLE:
			ev.m_Type = PlatformFormat_err;
			break;

		case GLFW_CURSOR_UNAVAILABLE:
			ev.m_Type = PlatformMouseCursorUnavailable_err;
			break;

		default:
			ev.m_Type = PlatformDriver_err;
			break;
		}

		char* buffer = (char*)malloc(QK_MAX_STRING_SIZE);
		if (!buffer) {
			ev.description() = nullptr;
			return;
		}

		strcpy_s(buffer, QK_MAX_STRING_SIZE, description);
		ev.description() = buffer;*/
	}






}