#pragma once


namespace qk::gfx::gui::io {

	enum MouseSource {
		Mouse,
		TouchScreen,
		Pen
	};
    // this is directly mapped to ImGuiKey for compatability purposes.
    enum Key : int
    {

        KeyNone = 0,

        KeyTab = 512,
        KeyLeftArrow,
        KeyRightArrow,
        KeyUpArrow,
        KeyDownArrow,
        KeyPageUp,
        KeyPageDown,
        KeyHome,
        KeyEnd,
        KeyInsert,
        KeyDelete,
        KeyBackspace,
        KeySpace,
        KeyEnter,
        KeyEscape,
        KeyLeftCtrl, KeyLeftShift, KeyLeftAlt, KeyLeftSuper,
        KeyRightCtrl, KeyRightShift, KeyRightAlt, KeyRightSuper,
        KeyMenu,
        Key0, Key1, Key2, Key3, Key4, Key5, Key6, Key7, Key8, Key9,
        KeyA, KeyB, KeyC, KeyD, KeyE, KeyF, KeyG, KeyH, KeyI, KeyJ,
        KeyK, KeyL, KeyM, KeyN, KeyO, KeyP, KeyQ, KeyR, KeyS, KeyT,
        KeyU, KeyV, KeyW, KeyX, KeyY, KeyZ,
        KeyF1, KeyF2, KeyF3, KeyF4, KeyF5, KeyF6,
        KeyF7, KeyF8, KeyF9, KeyF10, KeyF11, KeyF12,
        KeyF13, KeyF14, KeyF15, KeyF16, KeyF17, KeyF18,
        KeyF19, KeyF20, KeyF21, KeyF22, KeyF23, KeyF24,
        KeyApostrophe,
        KeyComma,
        KeyMinus,
        KeyPeriod,
        KeySlash,
        KeySemicolon,
        KeyEqual,
        KeyLeftBracket,
        KeyBackslash,
        KeyRightBracket,
        KeyGraveAccent,
        KeyCapsLock,
        KeyScrollLock,
        KeyNumLock,
        KeyPrintScreen,
        KeyPause,
        KeyKeypad0, KeyKeypad1, KeyKeypad2, KeyKeypad3, KeyKeypad4,
        KeyKeypad5, KeyKeypad6, KeyKeypad7, KeyKeypad8, KeyKeypad9,
        KeyKeypadDecimal,
        KeyKeypadDivide,
        KeyKeypadMultiply,
        KeyKeypadSubtract,
        KeyKeypadAdd,
        KeyKeypadEnter,
        KeyKeypadEqual,
        KeyAppBack,
        KeyAppForward,
        KeyOem102,


        KeyGamepadStart,
        KeyGamepadBack,
        KeyGamepadFaceLeft,
        KeyGamepadFaceRight,
        KeyGamepadFaceUp,
        KeyGamepadFaceDown,
        KeyGamepadDpadLeft,
        KeyGamepadDpadRight,
        KeyGamepadDpadUp,
        KeyGamepadDpadDown,
        KeyGamepadL1,
        KeyGamepadR1,
        KeyGamepadL2,
        KeyGamepadR2,
        KeyGamepadL3,
        KeyGamepadR3,
        KeyGamepadLStickLeft,
        KeyGamepadLStickRight,
        KeyGamepadLStickUp,
        KeyGamepadLStickDown,
        KeyGamepadRStickLeft,
        KeyGamepadRStickRight,
        KeyGamepadRStickUp,
        KeyGamepadRStickDown,

        
        
        KeyMouseLeft, KeyMouseRight, KeyMouseMiddle, KeyMouseX1, KeyMouseX2, KeyMouseWheelX, KeyMouseWheelY,

        
        KeyReservedForModCtrl, KeyReservedForModShift, KeyReservedForModAlt, KeyReservedForModSuper,

        
        
        Mod_None = 0,
        Mod_Ctrl = 1 << 12,
        Mod_Shift = 1 << 13,
        Mod_Alt = 1 << 14,
        Mod_Super = 1 << 15,


    };

	void PushMousePosEvent(float x, float y);

	void PushMouseButtonEvent(int button, bool pressed);

	void PushMouseWheelEvent(float wheel_x, float wheel_y);

	void PushKeyEvent(Key keycode, bool pressed);
	
	void PushInputCharacter(unsigned int c);

	void PushInputCharactersUTF8(const char* text);

	void PushFocusEvent(bool focused);

	void PushMouseSourceEvent(MouseSource source);

	void SetDisplaySize(float width, float height);

	void SetDeltaTime(float dt);

	bool& RequestsKeyboardInput();

	bool& RequestsMouseInput();

	void set_display_size(float w, float h);
}