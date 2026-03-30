/// **************************************** QUAKASTER ENGINE **************************************** 
/// core/platform/glfw_EventTranslator.h
/// Purpose: Defines function callbacks for converting GLFW events to Quakaster events.
/// ************************************************************************************************** 

#pragma once
#include <GLFW/glfw3.h>
#include "../../core.h"

namespace qk::io {
	void QK_API on_mouse_button(GLFWwindow* window, int button, int action, int modifiers);
	void QK_API on_key(GLFWwindow* window, int key, int scancode, int action, int modifiers);
	void QK_API on_mouse_move(GLFWwindow* window, double xpos, double ypos);
	void QK_API on_mouse_enter(GLFWwindow* window, int entered);
	void QK_API on_mouse_scroll(GLFWwindow* window, double xoffset, double yoffset);
	void QK_API on_window_resize(GLFWwindow* window, int width, int height);
	void QK_API on_window_close(GLFWwindow* window);
	void QK_API on_window_refresh(GLFWwindow* window);
	void QK_API on_window_focus(GLFWwindow* window, int focus);
	void QK_API on_window_minimize(GLFWwindow* window, int iconified);
	void QK_API on_window_maximize(GLFWwindow* window,int maximized);
	void QK_API on_framebuffer_resize(GLFWwindow* window, int width, int height);
	void QK_API on_window_DPI_update(GLFWwindow* window, float xscale, float yscale);
	void QK_API on_textinput(GLFWwindow* window, unsigned int codepoint);
	void QK_API on_textinputEx(GLFWwindow* window, unsigned int codepoint, int modifiers);
	void QK_API on_drag_drop(GLFWwindow* window, int pathcount, const char* paths[]);
	void QK_API on_joystick_connect(int jid, int event);
	void QK_API on_monitor_event(GLFWmonitor* window, int event);
	void QK_API on_glfw_error(int code, const char* description);

}

