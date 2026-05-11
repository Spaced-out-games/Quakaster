#pragma once
#include "HTexture.h"

namespace qk::gfx::rhi::gui {


	enum class SliderFlags
	{
		None = 0,
		Logarithmic = 1 << 5,       // Make the widget logarithmic (linear otherwise). Consider using ImGuiSliderFlags_NoRoundToFormat with this if using a format-string with small amount of digits.
		NoRoundToFormat = 1 << 6,       // Disable rounding underlying value to match precision of the display format string (e.g. %.3f values are rounded to those 3 digits).
		NoInput = 1 << 7,       // Disable Ctrl+Click or Enter key allowing to input text directly into the widget.
		WrapAround = 1 << 8,       // Enable wrapping around from max to min and from min to max. Only supported by DragXXX() functions for now.
		ClampOnInput = 1 << 9,       // Clamp value to min/max bounds when input manually with Ctrl+Click. By default Ctrl+Click allows going out of bounds.
		ClampZeroRange = 1 << 10,      // Clamp even if min==max==0.0f. Otherwise due to legacy reason DragXXX functions don't clamp with those values. When your clamping limits are dynamic you almost always want to use it.
		NoSpeedTweaks = 1 << 11,      // Disable keyboard modifiers altering tweak speed. Useful if you want to alter tweak speed yourself based on your own logic.
		ColorMarkers = 1 << 12,      // DragScalarN(), SliderScalarN(): Draw R/G/B/A color markers on each component.
		AlwaysClamp = ClampOnInput | ClampZeroRange,
		InvalidMask_ = 0x7000000F,   // [Internal] We treat using those bits as being potentially a 'float power' argument from legacy API (obsoleted 2020-08) that has got miscast to this enum, and will trigger an assert if needed.
	};


	struct Vec2 {
		float x;
		float y;
	};

	struct Vec3 {
		float x;
		float y;
		float z;
	};

	struct Color {
		float r;
		float g;
		float b;
	};

	void demo();


	// windowing
	bool begin(const char* name, bool* open = nullptr, int flags = 0);
	void end();

	bool begin_child(const char* id, const Vec2& size = {0,0}, bool border = false);
	void end_child();

	// layout
	void separator();
	void same_line();
	void new_line();
	void spacing();
	void indent(float amount = 0.0f);

	// text
	void text(const char* fmt);
	void text_colored(const Color& color, const char* fmt);
	void text_wrapped(const char* fmt);

	// buttons
	bool button(const char* label, const Vec2& size = { 0,0 });
	bool checkbox(const char* label, bool& value);
	bool radio_button(const char* label, bool active);

	// input
	bool input_text(const char* label, char* buffer, size_t size);
	bool input_float(const char* label, float& value, float step = 0.1f);

	// sliders
	bool slider_float(const char* label, float& v, float min, float max);
	bool slider_float3(const char* label, float v[3], float min, float max, const char* fmt, SliderFlags flags = SliderFlags::None);
	bool slider_int(const char* label, int& v, int min, int max);
	bool drag_float(const char* label, float& v, float speed = 1.0f);

	// combo
	bool combo(const char* label, int& current, const char* const items[], int count);
	bool selectable(const char* label, bool selected);

	// color
	bool color_edit4(const char* label, float rgba[4]);
	bool color_edit3(const char* label, float rgb[3]);

	// popups and menus
	bool begin_popup(const char* id);
	void end_popup();
	bool menu_item(const char* label, bool selected = false);

	void image(const HTexture& hTex, const Vec2& size, const Vec2& uv0, const Vec2& uv1);
	

}