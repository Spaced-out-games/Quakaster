#pragma once
#include <string>
#include <backends/imgui_impl_sdl2.h>
#include <backends/imgui_impl_opengl3.h>
#include "imgui.h"
#include <include/GameContext/UI/UIContext.h> // Include UIContext before implementing methods
#include <include/GameContext/UI/UIBase.h>
#include <format>


struct SpedometerUI: public UIBase
{
	glm::vec3& vel_ref;
	float maxspeed = 10.0f;
	SpedometerUI(glm::vec3& vel_ref) : vel_ref(vel_ref) { always_visible = 1; }
	virtual void draw(UIContext*) override {
		std::string string = std::format("{}u/s", glm::length(vel_ref));
//		std::string string = std::to_string((int)(vel_ref * 100.0f));

		ImGui::SetNextWindowPos(ImVec2(100, 100), ImGuiCond_Always); // Set position
		ImGui::SetNextWindowSize(ImVec2(200, 100), ImGuiCond_Always); // Set size

		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f); // Remove border
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0)); // Remove padding
		ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0, 0, 0, 0)); // Transparent background

		if (ImGui::Begin("Invisible Window", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse)) {
			ImGui::Text("Speed");
			ImGui::ProgressBar(glm::length(vel_ref) / maxspeed, ImVec2(-1, 0), string.c_str());
		}
		ImGui::End();

		ImGui::PopStyleVar(2);
		ImGui::PopStyleColor();
	}
};
