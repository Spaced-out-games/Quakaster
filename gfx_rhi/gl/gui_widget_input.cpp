#include "gui_widget_input.h"
#include "imgui.h"

namespace qk::gfx::gui::io {




	ImGuiMouseSource MouseSource_cast(MouseSource source) {
		switch (source)
		{
		case qk::gfx::gui::io::MouseSource::Mouse:
			return ImGuiMouseSource_Mouse;
			break;
		case qk::gfx::gui::io::MouseSource::TouchScreen:
			return ImGuiMouseSource_TouchScreen;
			break;
		case qk::gfx::gui::io::MouseSource::Pen:
			return ImGuiMouseSource_Pen;
			break;
		default:
			break;
		}
	}

	void PushMousePosEvent(float x, float y) {
		ImGuiIO& io = ImGui::GetIO();
		io.AddMousePosEvent(x, y);
	}

	void PushMouseButtonEvent(int button, bool pressed) {
		ImGuiIO& io = ImGui::GetIO();
		io.AddMouseButtonEvent(button, pressed);
	}

	void PushMouseWheelEvent(float wheel_x, float wheel_y) {
		ImGuiIO& io = ImGui::GetIO();
		io.AddMouseWheelEvent(wheel_x, wheel_y);
	}

	void PushKeyEvent(Key keycode, bool pressed) {
		ImGuiIO& io = ImGui::GetIO();
		ImGuiKey key = (ImGuiKey)keycode;
		io.AddKeyEvent((ImGuiKey)key, pressed);
	}

	void PushInputCharacter(unsigned int c) {
		ImGuiIO& io = ImGui::GetIO();
		io.AddInputCharacter(c);
	}

	void PushInputCharactersUTF8(const char* text) {
		ImGuiIO& io = ImGui::GetIO();
		io.AddInputCharactersUTF8(text);
	}

	void PushFocusEvent(bool focused) {
		ImGuiIO& io = ImGui::GetIO();
		io.AddFocusEvent(focused);
	}

	void PushMouseSourceEvent(MouseSource source) {
		ImGuiIO& io = ImGui::GetIO();
		io.AddMouseSourceEvent(MouseSource_cast(source));
	}

	void SetDisplaySize(float width, float height) {
		ImGuiIO& io = ImGui::GetIO();
		io.DisplaySize = ImVec2(width, height);
	}

	void SetDeltaTime(float dt) {
		ImGuiIO& io = ImGui::GetIO();
		io.DeltaTime = dt;
	}

	bool& RequestsKeyboardInput() {
		ImGuiIO& io = ImGui::GetIO();
		return io.WantCaptureKeyboard;
	}

	bool& RequestsMouseInput() {
		ImGuiIO& io = ImGui::GetIO();
		return io.WantCaptureMouse;
	}



}