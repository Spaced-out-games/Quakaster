#include "GUILayer.h"
#include <gui_widget_input.h>
#include <io/DefaultEvents.h>
#include <GLFW\glfw3.h>
#include <gui_rhi.h>




namespace qk::gfx::gui::io {

    Key key_cast(int glfw_key)
    {
        using K = Key;

        switch (glfw_key)
        {
            // --- Control keys ---
        case GLFW_KEY_TAB:        return K::KeyTab;
        case GLFW_KEY_LEFT:       return K::KeyLeftArrow;
        case GLFW_KEY_RIGHT:      return K::KeyRightArrow;
        case GLFW_KEY_UP:         return K::KeyUpArrow;
        case GLFW_KEY_DOWN:       return K::KeyDownArrow;
        case GLFW_KEY_PAGE_UP:    return K::KeyPageUp;
        case GLFW_KEY_PAGE_DOWN:  return K::KeyPageDown;
        case GLFW_KEY_HOME:       return K::KeyHome;
        case GLFW_KEY_END:        return K::KeyEnd;
        case GLFW_KEY_INSERT:     return K::KeyInsert;
        case GLFW_KEY_DELETE:     return K::KeyDelete;
        case GLFW_KEY_BACKSPACE:  return K::KeyBackspace;
        case GLFW_KEY_SPACE:      return K::KeySpace;
        case GLFW_KEY_ENTER:      return K::KeyEnter;
        case GLFW_KEY_ESCAPE:     return K::KeyEscape;

            // --- Modifiers ---
        case GLFW_KEY_LEFT_CONTROL:  return K::KeyLeftCtrl;
        case GLFW_KEY_RIGHT_CONTROL: return K::KeyRightCtrl;
        case GLFW_KEY_LEFT_SHIFT:    return K::KeyLeftShift;
        case GLFW_KEY_RIGHT_SHIFT:   return K::KeyRightShift;
        case GLFW_KEY_LEFT_ALT:      return K::KeyLeftAlt;
        case GLFW_KEY_RIGHT_ALT:     return K::KeyRightAlt;
        case GLFW_KEY_LEFT_SUPER:    return K::KeyLeftSuper;
        case GLFW_KEY_RIGHT_SUPER:   return K::KeyRightSuper;

            // --- Numbers ---
        case GLFW_KEY_0: return K::Key0;
        case GLFW_KEY_1: return K::Key1;
        case GLFW_KEY_2: return K::Key2;
        case GLFW_KEY_3: return K::Key3;
        case GLFW_KEY_4: return K::Key4;
        case GLFW_KEY_5: return K::Key5;
        case GLFW_KEY_6: return K::Key6;
        case GLFW_KEY_7: return K::Key7;
        case GLFW_KEY_8: return K::Key8;
        case GLFW_KEY_9: return K::Key9;

            // --- Letters ---
        case GLFW_KEY_A: return K::KeyA;
        case GLFW_KEY_B: return K::KeyB;
        case GLFW_KEY_C: return K::KeyC;
        case GLFW_KEY_D: return K::KeyD;
        case GLFW_KEY_E: return K::KeyE;
        case GLFW_KEY_F: return K::KeyF;
        case GLFW_KEY_G: return K::KeyG;
        case GLFW_KEY_H: return K::KeyH;
        case GLFW_KEY_I: return K::KeyI;
        case GLFW_KEY_J: return K::KeyJ;
        case GLFW_KEY_K: return K::KeyK;
        case GLFW_KEY_L: return K::KeyL;
        case GLFW_KEY_M: return K::KeyM;
        case GLFW_KEY_N: return K::KeyN;
        case GLFW_KEY_O: return K::KeyO;
        case GLFW_KEY_P: return K::KeyP;
        case GLFW_KEY_Q: return K::KeyQ;
        case GLFW_KEY_R: return K::KeyR;
        case GLFW_KEY_S: return K::KeyS;
        case GLFW_KEY_T: return K::KeyT;
        case GLFW_KEY_U: return K::KeyU;
        case GLFW_KEY_V: return K::KeyV;
        case GLFW_KEY_W: return K::KeyW;
        case GLFW_KEY_X: return K::KeyX;
        case GLFW_KEY_Y: return K::KeyY;
        case GLFW_KEY_Z: return K::KeyZ;

            // --- Function keys ---
        case GLFW_KEY_F1:  return K::KeyF1;
        case GLFW_KEY_F2:  return K::KeyF2;
        case GLFW_KEY_F3:  return K::KeyF3;
        case GLFW_KEY_F4:  return K::KeyF4;
        case GLFW_KEY_F5:  return K::KeyF5;
        case GLFW_KEY_F6:  return K::KeyF6;
        case GLFW_KEY_F7:  return K::KeyF7;
        case GLFW_KEY_F8:  return K::KeyF8;
        case GLFW_KEY_F9:  return K::KeyF9;
        case GLFW_KEY_F10: return K::KeyF10;
        case GLFW_KEY_F11: return K::KeyF11;
        case GLFW_KEY_F12: return K::KeyF12;
        case GLFW_KEY_F13: return K::KeyF13;
        case GLFW_KEY_F14: return K::KeyF14;
        case GLFW_KEY_F15: return K::KeyF15;
        case GLFW_KEY_F16: return K::KeyF16;
        case GLFW_KEY_F17: return K::KeyF17;
        case GLFW_KEY_F18: return K::KeyF18;
        case GLFW_KEY_F19: return K::KeyF19;
        case GLFW_KEY_F20: return K::KeyF20;
        case GLFW_KEY_F21: return K::KeyF21;
        case GLFW_KEY_F22: return K::KeyF22;
        case GLFW_KEY_F23: return K::KeyF23;
        case GLFW_KEY_F24: return K::KeyF24;

            // --- Punctuation ---
        case GLFW_KEY_APOSTROPHE:    return K::KeyApostrophe;
        case GLFW_KEY_COMMA:         return K::KeyComma;
        case GLFW_KEY_MINUS:         return K::KeyMinus;
        case GLFW_KEY_PERIOD:        return K::KeyPeriod;
        case GLFW_KEY_SLASH:         return K::KeySlash;
        case GLFW_KEY_SEMICOLON:     return K::KeySemicolon;
        case GLFW_KEY_EQUAL:         return K::KeyEqual;
        case GLFW_KEY_LEFT_BRACKET:  return K::KeyLeftBracket;
        case GLFW_KEY_BACKSLASH:     return K::KeyBackslash;
        case GLFW_KEY_RIGHT_BRACKET: return K::KeyRightBracket;
        case GLFW_KEY_GRAVE_ACCENT:  return K::KeyGraveAccent;

            // --- Locks / system ---
        case GLFW_KEY_CAPS_LOCK:     return K::KeyCapsLock;
        case GLFW_KEY_SCROLL_LOCK:   return K::KeyScrollLock;
        case GLFW_KEY_NUM_LOCK:      return K::KeyNumLock;
        case GLFW_KEY_PRINT_SCREEN:  return K::KeyPrintScreen;
        case GLFW_KEY_PAUSE:         return K::KeyPause;

            // --- Keypad ---
        case GLFW_KEY_KP_0: return K::KeyKeypad0;
        case GLFW_KEY_KP_1: return K::KeyKeypad1;
        case GLFW_KEY_KP_2: return K::KeyKeypad2;
        case GLFW_KEY_KP_3: return K::KeyKeypad3;
        case GLFW_KEY_KP_4: return K::KeyKeypad4;
        case GLFW_KEY_KP_5: return K::KeyKeypad5;
        case GLFW_KEY_KP_6: return K::KeyKeypad6;
        case GLFW_KEY_KP_7: return K::KeyKeypad7;
        case GLFW_KEY_KP_8: return K::KeyKeypad8;
        case GLFW_KEY_KP_9: return K::KeyKeypad9;

        case GLFW_KEY_KP_DECIMAL: return K::KeyKeypadDecimal;
        case GLFW_KEY_KP_DIVIDE:  return K::KeyKeypadDivide;
        case GLFW_KEY_KP_MULTIPLY:return K::KeyKeypadMultiply;
        case GLFW_KEY_KP_SUBTRACT:return K::KeyKeypadSubtract;
        case GLFW_KEY_KP_ADD:     return K::KeyKeypadAdd;
        case GLFW_KEY_KP_ENTER:   return K::KeyKeypadEnter;
        case GLFW_KEY_KP_EQUAL:   return K::KeyKeypadEqual;

            // --- Misc ---
        case GLFW_KEY_MENU: return K::KeyMenu;
        //case GLFW_KEY_APP_BACK: return K::KeyAppBack;
        //case GLFW_KEY_APP_FORWARD: return K::KeyAppForward;
        case GLFW_KEY_UNKNOWN: return K::KeyNone;

        default:
            return K::KeyNone;
        }
    }

}


namespace qk {

    template <>
    inline GLFWwindow* get<WindowTag, GLFWwindow*>(const Handle<WindowTag>& hdl) {
        return reinterpret_cast<GLFWwindow*>(hdl.m_Handle);
    }

    template <>
    inline void set<WindowTag, GLFWwindow*>(Handle<WindowTag>& hdl, GLFWwindow* value) {
        hdl.m_Handle = reinterpret_cast<uintptr_t>(value);
    }
}

namespace qk::gfx::gui {

    GUILayer::GUILayer(qk::HWindow hWindow) {
        
        qk::set<WindowTag, GLFWwindow*>(m_hWindow, qk::get<WindowTag, GLFWwindow*>(hWindow));

    }


	void GUILayer::on_render() {
        for (size_t i = 0; i < m_Widgets.size(); i++) {
            m_Widgets[i]->render();
        }
	}

	void GUILayer::on_attach() {
        subscribe(EEventCategory::cat_kbmouse);
        rhi::gui::init(m_hWindow);
	}

	void GUILayer::on_detach() {

	}

	[[nodiscard]] Layer::EBlock GUILayer::on_event(const Event& evt) {
        

        switch (evt.m_Type)
        {

        case EEventType::KeyPress_evt:
        case EEventType::KeyRelease_evt:
        case EEventType::KeyHold_evt:
        {
            const KeyEvent& e = (const KeyEvent&)evt;

            gui::io::Key keycode = gui::io::key_cast(e.key());
            bool down = (evt.m_Type != EEventType::KeyRelease_evt);

            if (keycode != gui::io::KeyNone)
                io::PushKeyEvent(keycode, down);
            io::PushKeyEvent(gui::io::KeyLeftCtrl, (e.mods() & GLFW_MOD_CONTROL) != 0);
            io::PushKeyEvent(gui::io::KeyLeftShift, (e.mods() & GLFW_MOD_SHIFT) != 0);
            io::PushKeyEvent(gui::io::KeyLeftAlt, (e.mods() & GLFW_MOD_ALT) != 0);
            io::PushKeyEvent(gui::io::KeyLeftSuper, (e.mods() & GLFW_MOD_SUPER) != 0);
            break;
        }


        case EEventType::TextInput_evt:
        {
            const TextInputEvent& e = (const TextInputEvent&)evt;
            io::PushInputCharacter(e.code());
            break;
        }


        case EEventType::MousePress_evt:
        case EEventType::MouseRelease_evt:
        case EEventType::MouseHold_evt:
        {
            const MouseEvent& e = (const MouseEvent&)evt;

            int button = e.button();
            bool down = (evt.m_Type != EEventType::MouseRelease_evt);

            io::PushMouseButtonEvent(button, down);
            break;
        }


        case EEventType::MouseMove_evt:
        {
            const MouseMoveEvent& e = (const MouseMoveEvent&)evt;
            io::PushMousePosEvent(e.x(), e.y());
            break;
        }


        case EEventType::MouseScroll_evt:
        {
            const MouseScrollEvent& e = (const MouseScrollEvent&)evt;
            io::PushMouseWheelEvent(e.dx(), e.dy());
            break;
        }


        case EEventType::WindowResize_evt:
        case EEventType::FramebufferResize_evt:
        {
            const WindowFramebufferResizeEvent& e = (const WindowFramebufferResizeEvent&)evt;
            io::SetDisplaySize(e.width(), e.height());
            break;
        }

        default:
            break;
        }


        if (io::RequestsKeyboardInput() || io::RequestsMouseInput())
            return EBlock::Block;

        return EBlock::Pass;
    }
}