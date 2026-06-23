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

    // Push event to imgui
	void PushMousePosEvent(float x, float y);

    // Push event to imgui
	void PushMouseButtonEvent(int button, bool pressed);
    
    // Push event to imgui
	void PushMouseWheelEvent(float wheel_x, float wheel_y);

    // Push event to imgui
    void PushKeyEvent(Key keycode, bool pressed);
	
    // Push event to imgui
    void PushInputCharacter(unsigned int c);

    // Push event to imgui
    void PushInputCharactersUTF8(const char* text);

    // Push event to imgui
    void PushFocusEvent(bool focused);

    // Push event to imgui
    void PushMouseSourceEvent(MouseSource source);

    // Tell ImGui what the display size is
    void SetDisplaySize(float width, float height);

    // Tell ImGui the deltaTime
    void SetDeltaTime(float dt);

    // Whether or not ImGui wants to consume keyboard input
    bool& RequestsKeyboardInput();

    // Whether or not ImGui wants to consume mouse input
    bool& RequestsMouseInput();

    // Tell ImGui what the display size is. Might be a duplicate lol
    //void setDisplaySize(float w, float h);
}