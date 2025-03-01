#pragma once
#include <string>
#include <backends/imgui_impl_sdl2.h>
#include <backends/imgui_impl_opengl3.h>
#include "imgui.h"
#include <include/GameContext/UI/UIContext.h> // Include UIContext before implementing methods
#include <include/GameContext/UI/UIBase.h>


struct spedometer : UIBase
{
	spedometer (glm::vec3& target_velocity, float maxspeed): velocity_vector(target_velocity), maxspeed(maxspeed) {
		always_visible = true;
	}

	glm::vec3& velocity_vector;
	float maxspeed =10.0f;

	void draw(UIContext* ctx) override {
		std::string string = std::to_string((int)(glm::length(velocity_vector) * 100.0f));

		ImGui::SetNextWindowPos(ImVec2(100, 100), ImGuiCond_Always); // Set position
		ImGui::SetNextWindowSize(ImVec2(200, 100), ImGuiCond_Always); // Set size

		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f); // Remove border
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0)); // Remove padding
		ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0, 0, 0, 0)); // Transparent background

		if (ImGui::Begin("Invisible Window", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse)) {
			ImGui::Text("Speed");
			ImGui::ProgressBar(glm::length(velocity_vector) / maxspeed, ImVec2(-1, 0), string.c_str());
		}
		ImGui::End();

		ImGui::PopStyleVar(2);
		ImGui::PopStyleColor();
	}
};