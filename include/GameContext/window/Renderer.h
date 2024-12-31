#pragma once
#include <SDL.h>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <imgui.h>

struct Renderer
{
	SDL_GLContext gl_context = nullptr;
	ImGuiContext* gui_context = nullptr;
};