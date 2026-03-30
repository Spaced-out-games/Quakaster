#include "DefaultEvents.h"

namespace qk::io {
	std::string to_string(const Event& evt) {
		using enum EEventType;
		Event& e = const_cast<Event&>(evt);

		if ((uint32_t)e.m_Type >= CUSTOM_EVENT_BEGIN)
		{
			if (g_EventExToString_pfn)
			{
				std::string s = g_EventExToString_pfn(evt);
				if (!s.empty())
					return s;
			}

			return std::format("Custom Event #{}", (uint32_t)(evt.m_Type) - CUSTOM_EVENT_BEGIN);
		}


		switch (e.m_Type)
		{
		case Null_evt: return "NullEvent";
		case KeyPress_evt:
		{
			auto& ev = (KeyPressEvent&)e;
			return std::format("KeyPressEvent {{ key={}, scancode={}, mods={} }}", ev.key(), ev.scancode(), ev.mods());
		}
		case KeyRelease_evt:
		{
			auto& ev = (KeyReleaseEvent&)e;
			return std::format("KeyReleaseEvent {{ key={}, scancode={}, mods={} }}", ev.key(), ev.scancode(), ev.mods());
		}
		case KeyHold_evt:
		{
			auto& ev = (KeyHoldEvent&)e;
			return std::format("KeyHoldEvent {{ key={}, scancode={}, mods={} }}", ev.key(), ev.scancode(), ev.mods());
		}
		case MousePress_evt:
		{
			auto& ev = (MousePressEvent&)e;
			return std::format("MousePressEvent {{ btn={}, mods={} }}", ev.button(), ev.mods());
		}
		case MouseHold_evt:
		{
			auto& ev = (MouseHoldEvent&)e;
			return std::format("MouseHoldEvent {{ btn={}, mods={} }}", ev.button(), ev.mods());
		}
		case MouseRelease_evt:
		{
			auto& ev = (MouseReleaseEvent&)e;
			return std::format("MouseReleaseEvent {{ btn={}, mods={} }}", ev.button(), ev.mods());
		}
		case MouseMove_evt:
		{
			auto& ev = (MouseMoveEvent&)e;
			return std::format("MouseMoveEvent {{ {}, {} }}", ev.x(), ev.y());
		}
		case MouseScroll_evt:
		{
			auto& ev = (MouseScrollEvent&)e;
			return std::format("MouseScrollEvent {{ {}, {} }}", ev.dx(), ev.dy());
		}
		case MouseEnter_evt: return "MouseEnterEvent";
		case MouseExit_evt: return "MouseExitEvent";
		case WindowResize_evt:
		{
			auto& ev = (WindowResizeEvent&)e;
			return std::format("WindowResizeEvent {{ {}, {} }}", ev.width(), ev.height());
		}
		case FramebufferResize_evt:
		{
			auto& ev = (WindowFramebufferResizeEvent&)e;
			return std::format("FramebufferResizeEvent {{ {}, {} }}", ev.width(), ev.height());
		}
		case WindowDPIUpdate_evt:
		{
			auto& ev = (WindowDPIUpdateEvent&)e;
			return std::format("WindowDPIUpdateEvent {{ {}, {} }}", ev.xscale(), ev.yscale());
		}
		case DragDrop_evt:
		{
			auto& ev = (DragDropEvent&)e;
			std::string s = std::format("DragDropEvent count={} paths: {{\n", ev.count());
			for (int i = 0; i < ev.count(); ++i) s += std::format("\t\"{}\"\n", ev.paths(i));
			s += "}";
			return s;
		}
		case JoystickConnect_evt:
		{
			auto& ev = (JoystickConnectEvent&)e;
			return std::format("JoystickConnectEvent jid={}", ev.jid());
		}
		case JoystickDisconnect_evt:
		{
			auto& ev = (JoystickDisconnectEvent&)e;
			return std::format("JoystickDisconnectEvent jid={}", ev.jid());
		}
		case MonitorConnect_evt: return "MonitorConnectEvent";
		case MonitorDisconnect_evt: return "MonitorDisconnectEvent";
		case PlatformUninitialized_err:
		case PlatformNoContext_err:
		case PlatformInvalidValue_err:
		case PlatformOutOfMemory_err:
		case PlatformInterfaceUnavailable_err:
		case PlatformVersionUnavailable_err:
		case PlatformDriver_err:
		case PlatformFormat_err:
		case PlatformMouseCursorUnavailable_err:
		{
			auto& ev = (Error&)e;
			if (ev.description()) return std::format("PlatformError: {}", ev.description());
			return "PlatformError";
		}
		default:
			return std::format("Unknown Event ({})", static_cast<uint32_t>(evt.m_Type));
		}
	}

}