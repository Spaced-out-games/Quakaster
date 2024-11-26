#pragma once
#include <SDL.h>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <imgui.h>
#include <backends/imgui_impl_sdl2.h>
#include <backends/imgui_impl_opengl3.h>
#include <iostream>
#include <include/FrameBuffer.h>
#include <include/CameraComponent.h>

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
    void endImGuiFrame();
    void renderImGui();
    void cleanupImGui();
    void renderToTexture();
    GLuint renderTexture = 0;

private:
    void createFramebuffer();

    SDL_Window* window = nullptr;
    SDL_GLContext context = nullptr;
    glm::ivec2 dimensions;
    ImGuiContext* ImGUIContext = nullptr;
    ImGuiIO* ImGUI_IO = nullptr;
    bool running = true;
    CameraComponent* activeCamera;
    FrameBuffer viewport;
};
