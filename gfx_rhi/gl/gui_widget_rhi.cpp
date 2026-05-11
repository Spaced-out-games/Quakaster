#include "gui_widget_rhi.h"
#include "imgui.h"

namespace qk::gfx::rhi::gui {

    ImGuiSliderFlags SliderFlags_cast(SliderFlags flags) {
        return ImGuiSliderFlags(flags);
    }

    // -------------------- demo --------------------

    void demo()
    {
        ImGui::ShowDemoWindow();
    }

    // -------------------- windowing --------------------

    bool begin(const char* name, bool* open, int flags)
    {
        return ImGui::Begin(name, open, flags);
    }

    void end()
    {
        ImGui::End();
    }

    bool begin_child(const char* id, const Vec2& size, bool border)
    {
        return ImGui::BeginChild(id, ImVec2(size.x, size.y), border);
    }

    void end_child()
    {
        ImGui::EndChild();
    }

    // -------------------- layout --------------------

    void separator()
    {
        ImGui::Separator();
    }

    void same_line()
    {
        ImGui::SameLine();
    }

    void new_line()
    {
        ImGui::NewLine();
    }

    void spacing()
    {
        ImGui::Spacing();
    }

    void indent(float amount)
    {
        ImGui::Indent(amount);
    }

    // -------------------- text --------------------

    void text(const char* fmt)
    {
        ImGui::TextUnformatted(fmt);
    }

    void text_colored(const Color& color, const char* fmt)
    {
        ImGui::TextColored(ImVec4(color.r, color.g, color.b, 1.0f), "%s", fmt);
    }

    void text_wrapped(const char* fmt)
    {
        ImGui::TextWrapped("%s", fmt);

    }

    // -------------------- buttons --------------------

    bool button(const char* label, const Vec2& size)
    {
        return ImGui::Button(label, ImVec2(size.x, size.y));
    }

    bool checkbox(const char* label, bool& value)
    {
        return ImGui::Checkbox(label, &value);
    }

    bool radio_button(const char* label, bool active)
    {
        return ImGui::RadioButton(label, active);
    }

    // -------------------- input --------------------

    bool input_text(const char* label, char* buffer, size_t size)
    {
        return ImGui::InputText(label, buffer, size);
    }

    bool input_float(const char* label, float& value, float step)
    {
        return ImGui::InputFloat(label, &value, step);
    }

    // -------------------- sliders --------------------

    bool slider_float(const char* label, float& v, float min, float max)
    {
        return ImGui::SliderFloat(label, &v, min, max);
    }
    bool slider_float3(const char* label, float v[3], float min, float max, const char* fmt, SliderFlags flags) {

        return ImGui::SliderFloat3(label, v, min, max, fmt, SliderFlags_cast(flags));
    }

    bool slider_int(const char* label, int& v, int min, int max)
    {
        return ImGui::SliderInt(label, &v, min, max);
    }

    bool drag_float(const char* label, float& v, float speed)
    {
        return ImGui::DragFloat(label, &v, speed);
    }

    // -------------------- combo --------------------

    bool combo(const char* label, int& current, const char* const items[], int count)
    {
        return ImGui::Combo(label, &current, items, count);
    }

    bool selectable(const char* label, bool selected)
    {
        return ImGui::Selectable(label, selected);
    }

    // -------------------- color --------------------

    bool color_edit4(const char* label, float rgba[4])
    {
        return ImGui::ColorEdit4(label, rgba);
    }

    bool color_edit3(const char* label, float rgb[3])
    {
        return ImGui::ColorEdit3(label, rgb);
    }

    // -------------------- popups / menus --------------------

    bool begin_popup(const char* id)
    {
        return ImGui::BeginPopup(id);
    }

    void end_popup()
    {
        ImGui::EndPopup();
    }

    bool menu_item(const char* label, bool selected)
    {
        return ImGui::MenuItem(label, nullptr, selected);
    }

    void image(const HTexture& hTex, const Vec2& size, const Vec2& uv0, const Vec2& uv1) {

        ImGui::Image(
            ImTextureRef((ImTextureID)hTex.value()),
            ImVec2{ size.x, size.y },
            ImVec2{ uv0.x, uv0.y },
            ImVec2{ uv1.x, uv1.y }
        );
    }




} // namespace qk::gfx::rhi::gui