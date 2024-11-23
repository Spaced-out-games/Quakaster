#pragma once
#include <SDL.h>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include "imgui.h"
#include "backends/imgui_impl_sdl2.h"
#include "backends/imgui_impl_opengl3.h"
#include <iostream>

class Window {
public:
    Window(int width, int height);
    ~Window();

    void swapBuffers();
    void resize(int new_width, int new_height);
    bool is_running() const { return running; }
    void quit() { running = false; }
    SDL_Window* getWindow() const { return window; }
    SDL_GLContext getContext() const { return context; }

    void beginImGuiFrame();
    void renderImGui();
    void cleanupImGui();

private:
    void createFramebuffer();

    SDL_Window* window = nullptr;
    SDL_GLContext context = nullptr;
    glm::ivec2 dimensions;
    bool running = true;

    GLuint fbo = 0;
    GLuint renderTexture = 0;
    GLuint rbo = 0;
};
