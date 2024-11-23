#pragma once

#include <SDL.h>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <iostream>

class Window {
    bool running = true;
    SDL_Window* window = nullptr;    
    SDL_GLContext context;
    glm::ivec2 dimensions;

    // OpenGL framebuffer, texture, and renderbuffer
    GLuint fbo = 0;         // Framebuffer Object
    GLuint renderTexture = 0; // Texture attached to FBO
    GLuint rbo = 0;         // Renderbuffer for depth and stencil

public:
    ~Window();
    Window(int width, int height);
    void swapBuffers();
    const bool& is_running() { return running; }
    const SDL_Window* get_window() { return window; }
    const SDL_GLContext get_GLContext() { return context; }
    const glm::ivec2 get_dimensions() { return dimensions; }
    void quit() { running = false; }

    void resize(int new_width, int new_height);
    GLuint getRenderTexture() const { return renderTexture; }
    GLuint getFramebuffer() const { return fbo; }

private:
    void createFramebuffer();
};
