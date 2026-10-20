/// **************************************** QUAKASTER ENGINE **************************************** 
/// core/Window.h
/// Purpose: Impliments a basic Window wrapper for GLFW calls.
/// ************************************************************************************************** 
#include "Window.h"
#include <GLFW/glfw3.h>
#include "../io/DefaultEvents.h"
#include <utility/time.h>
//#include "../platform/glfw_EventTranslator.h"
#include "spdlog/spdlog.h"
#include <stdexcept>

using qk::utility::now;



namespace qk {



	
	

	void Window::setEventQueue(EventQueue* queue) {
		m_pQueue = queue;
	}
	

	EventQueue* Window::queue() {
		return m_pQueue;
	}

	void Window::makeContextCurrent() {
		auto* win = (GLFWwindow*)m_hWindow;

		if (!m_hWindow) {
			throw std::runtime_error("Window not initialized!");
		}
		glfwMakeContextCurrent(win);
	}

	Window::Position Window::getPosition() {
		auto* win = (GLFWwindow*)m_hWindow;

		Position result;
		glfwGetWindowPos(win, &(result.x), &(result.y));
		return result;
	}

	void Window::setPosition(Window::Position new_position) {
		auto* win = (GLFWwindow*)m_hWindow;

		
		glfwSetWindowPos(win, new_position.x, new_position.y);
	}

	std::string Window::getTitle() {
		return m_Title;
	}

	void Window::pollEvents() {
		glfwPollEvents();
	}




	int Window::shouldClose() {
		auto* win = (GLFWwindow*)m_hWindow;

		return glfwWindowShouldClose(win);
	}

	void Window::swapBuffers() {
		auto* win = (GLFWwindow*)m_hWindow;

		glfwSwapBuffers(win);
	}
	void Window::resize(Window::Size new_size) {
		auto* win = (GLFWwindow*)m_hWindow;

		glfwSetWindowSize(win, new_size.w, new_size.h);
	}

	Window::Size Window::getSize() const {
		auto* win = (GLFWwindow*)m_hWindow;

		Window::Size sz;
		glfwGetWindowSize(win, &sz.w, &sz.h);
		return sz;
	}

	HWindow Window::handle() {
		return m_hWindow;
	}




	void Window::init(Window::Size initial_size, const std::string& title, Window* shared) {
		auto* win = (GLFWwindow*)m_hWindow;

		spdlog::info("Window@{}: initialized", (const void*)this);

		#ifdef _DEBUG
		glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
		#endif


		m_Title = title;

		GLFWwindow* pWindow = glfwCreateWindow(initial_size.w, initial_size.h, title.c_str(),
			nullptr,//(GLFWmonitor*)monitor,
			nullptr);// shared ? shared->m_Window : nullptr);

		if (!pWindow) {
			spdlog::info("Window@{}: Failed to initialize GLFW window", (const void*)this);
			return;
		}
		m_hWindow = pWindow;

		// Now m_Window is valid & safe to set user pointer and callbacks
		glfwSetWindowUserPointer(pWindow, this);
		glfwMakeContextCurrent(pWindow);
		InitWindowEventSystem(*this);
	}

	Window::Window() {

	}

	void Window::destroy() {
		auto* win = (GLFWwindow*)m_hWindow;

		if (win) {
			spdlog::info("Window@{}: dtor called", (const void*)this);

			glfwDestroyWindow(win);
			m_hWindow = nullptr;
		}
	}


	Window::~Window() {
		destroy();
	}



	// EVENT IMPLEMENTATIONS



	/// **************************************** QUAKASTER ENGINE **************************************** 
/// core/platform/glfw_EventTranslator.h
/// Purpose: Implements function callbacks for converting GLFW events to Quakaster 
/// events and loading them into a queue.
/// ************************************************************************************************** 







	void on_key(GLFWwindow* window, int key, int scancode, int action, int mods) {
		
		using enum EEventCategory;
		using enum EEventType;
		Window* target = (Window*)glfwGetWindowUserPointer(window);
		auto* queue = target->queue();
		if (!queue) return;



		KeyEvent& kv = (KeyEvent&)queue->emplace_back();
		kv.m_Category = cat_keyboard;
		switch (action) {
		case GLFW_PRESS:  kv.m_Type = KeyPress_evt; break;
		case GLFW_REPEAT: kv.m_Type = KeyHold_evt;  break;
		case GLFW_RELEASE:kv.m_Type = KeyRelease_evt; break;
		}


		kv.timestamp = utility::now();
		kv.scancode() = scancode;
		kv.mods() = mods;
		kv.key() = key;

	}

	void on_mouse_button(GLFWwindow* window, int button, int action, int mods)
	{
		
		using enum EEventCategory;
		using enum EEventType;

		Window* target = (Window*)glfwGetWindowUserPointer(window);
		auto* queue = target->queue();
		if (!queue) return;

		MouseEvent& ev = (MouseEvent&)queue->emplace_back();
		ev.m_Category = cat_mouse;

		ev.timestamp = now();
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
		Window* target = (Window*)glfwGetWindowUserPointer(window);
		auto* queue = target->queue();
		if (!queue) return;

		MouseMoveEvent& mv = (MouseMoveEvent&)queue->emplace_back();
		mv.timestamp = now();
		mv.m_Category = cat_continuous | cat_mouse;
		mv.m_Type = MouseMove_evt;
		mv.x() = (float)xpos;
		mv.y() = (float)ypos;
		
	}

	void on_mouse_enter(GLFWwindow* window, int entered) {
		
		using enum EEventCategory;
		using enum EEventType;
		Window* target = (Window*)glfwGetWindowUserPointer(window);
		auto* queue = target->queue();
		if (!queue) return;

		Event& mv = queue->emplace_back();
		mv.m_Category = cat_mouse | cat_window;
		mv.timestamp = now();

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
		Window* target = (Window*)glfwGetWindowUserPointer(window);
		auto* queue = target->queue();
		if (!queue) return;
		MouseScrollEvent& sv = (MouseScrollEvent&)queue->emplace_back();
		sv.timestamp = now();

		sv.m_Category = cat_mouse | cat_continuous;
		sv.m_Type = MouseScroll_evt;
		sv.dx() = (float)xoffset;
		sv.dy() = (float)yoffset;
		
	}

	void on_window_resize(GLFWwindow* window, int width, int height) {
		
		using enum EEventCategory;
		using enum EEventType;
		Window* target = (Window*)glfwGetWindowUserPointer(window);
		auto* queue = target->queue();
		if (!queue) return;
		WindowResizeEvent& rv = (WindowResizeEvent&)queue->emplace_back();
		rv.timestamp = now();

		rv.m_Category = cat_window | cat_continuous;
		rv.m_Type = WindowResize_evt;
		rv.height() = height;
		rv.width() = width;
		
	}

	void on_window_close(GLFWwindow* window) {
		using enum EEventCategory;
		using enum EEventType;
		Window* target = (Window*)glfwGetWindowUserPointer(window);
		auto* queue = target->queue();
		if (!queue) return;
		WindowCloseEvent& cv = (WindowCloseEvent&)queue->emplace_back();
		cv.timestamp = now();

		cv.m_Category = cat_window;
		cv.m_Type = WindowClose_evt;
	}

	void on_window_refresh(GLFWwindow* window) { 
		using enum EEventCategory;
		using enum EEventType;
		Window* target = (Window*)glfwGetWindowUserPointer(window);
		auto* queue = target->queue();
		if (!queue) return;
		WindowRefreshEvent& rv = (WindowRefreshEvent&)queue->emplace_back();
		rv.m_Category = cat_window;
		rv.timestamp = now();

		rv.m_Type = WindowRefresh_evt;
	}

	void on_window_focus(GLFWwindow* window, int focus) { 
		using enum EEventCategory;
		using enum EEventType;
		Window* target = (Window*)glfwGetWindowUserPointer(window);
		auto* queue = target->queue();
		if (!queue) return;
		Event& ev = queue->emplace_back();
		ev.timestamp = now();

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
		Window* target = (Window*)glfwGetWindowUserPointer(window);
		auto* queue = target->queue();
		if (!queue) return;
		Event& ev = queue->emplace_back();
		ev.timestamp = now();

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
		Window* target = (Window*)glfwGetWindowUserPointer(window);
		auto* queue = target->queue();

		if (!queue) return;
		Event& ev = queue->emplace_back();
		ev.timestamp = now();

		ev.m_Category = cat_window;
		ev.m_Type = maximized ? WindowMaximize_evt : WindowMaximizeRestored_evt;
	}

	void on_framebuffer_resize(GLFWwindow* window, int width, int height) {
		
		// NOTICE: This couples us to OpenGL, but until we have some kind of viewport flushing
		// mechanism going, this will have to work.
		//glViewport(0, 0, width, height);
		using enum EEventCategory;
		using enum EEventType;
		Window* target = (Window*)glfwGetWindowUserPointer(window);
		auto* queue = target->queue();

		if (!queue) return;
		WindowFramebufferResizeEvent& ev = (WindowFramebufferResizeEvent&)queue->emplace_back();
		ev.timestamp = now();

		ev.m_Category = cat_window;
		ev.m_Type = FramebufferResize_evt;

		ev.width() = width;
		ev.height() = height;
		
	}

	void on_window_DPI_update(GLFWwindow* window, float xscale, float yscale) { 
		using enum EEventCategory;
		using enum EEventType;
		Window* target = (Window*)glfwGetWindowUserPointer(window);
		auto* queue = target->queue();

		if (!queue) return;
		WindowDPIUpdateEvent& ev = (WindowDPIUpdateEvent&)queue->emplace_back();
		ev.timestamp = now();
		ev.m_Category = cat_window;
		ev.m_Type = WindowDPIUpdate_evt;
		ev.xscale() = (float)xscale;
		ev.yscale() = (float)yscale;
	}
	void on_textinput(GLFWwindow* window, unsigned int codepoint) { 
		using enum EEventCategory;
		using enum EEventType;
		Window* target = (Window*)glfwGetWindowUserPointer(window);
		auto* queue = target->queue();

		if (!queue) return;
		TextInputEvent& ev = (TextInputEvent&)queue->emplace_back();
		ev.timestamp = now();
		ev.m_Category = cat_text;
		ev.m_Type = TextInput_evt;
		ev.code() = codepoint;
	
	}

	void on_textinputEx(GLFWwindow* window, unsigned int codepoint, int modifiers) {
		
		using enum EEventCategory;
		using enum EEventType;
		Window* target = (Window*)glfwGetWindowUserPointer(window);
		auto* queue = target->queue();

		if (!queue) return;
		TextInputEventEx& ev = (TextInputEventEx&)queue->emplace_back();
		ev.timestamp = now();
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

		Window* target = (Window*)glfwGetWindowUserPointer(window);
		auto* queue = target->queue();
		if (!queue) return;

		DragDropEvent& ev = (DragDropEvent&)queue->emplace_back();
		ev.timestamp = now();
		ev.m_Category = cat_window;
		ev.m_Type = DragDrop_evt;
		ev.count() = pathcount;

		char* pool = (char*)malloc(QK_MAX_STRING_SIZE * pathcount);
		if (!pool) return;

		for (int i = 0; i < pathcount; i++)
		{
			char* dst = pool + (QK_MAX_STRING_SIZE * i);
			//strcpy(dst, QK_MAX_STRING_SIZE, paths[i]);
			strncpy(dst, paths[i], QK_MAX_STRING_SIZE - 1);
			dst[QK_MAX_STRING_SIZE - 1] = '\0';
		}

		ev.paths() = pool;
		
	}


	// these were already commented out
	void on_joystick_connect(int jid, int event) {
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



	void InitWindowEventSystem(Window& window) {

		auto* win = (GLFWwindow*)window.handle();


		// Window events
		glfwSetWindowSizeCallback(win, on_window_resize);
		glfwSetFramebufferSizeCallback(win, on_framebuffer_resize);
		glfwSetWindowCloseCallback(win, on_window_close);
		glfwSetWindowRefreshCallback(win, on_window_refresh);
		glfwSetWindowFocusCallback(win, on_window_focus);
		glfwSetWindowIconifyCallback(win, on_window_minimize);
		glfwSetWindowMaximizeCallback(win, on_window_maximize);
		glfwSetWindowContentScaleCallback(win, on_window_DPI_update);

		// Input events
		glfwSetKeyCallback(win, on_key);
		glfwSetCharCallback(win, on_textinput);
		glfwSetCharModsCallback(win, on_textinputEx);

		glfwSetMouseButtonCallback(win, on_mouse_button);
		glfwSetCursorPosCallback(win, on_mouse_move);
		glfwSetCursorEnterCallback(win, on_mouse_enter);
		glfwSetScrollCallback(win, on_mouse_scroll);

		// Drag & drop
		glfwSetDropCallback(win, on_drag_drop);

		// Joystick connection
		glfwSetJoystickCallback(on_joystick_connect);
	}

	Window::operator SymbolLoader() {
		return (SymbolLoader)glfwGetProcAddress;
	}






}

