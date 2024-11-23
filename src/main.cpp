// third-party
#include <SDL.h>
#include <GL/glew.h>
#include <GL/gl.h>
#include <entt/entt.hpp>
#include <iostream>
#include <include/window.h>
#include <include/TextureComponent.h>









bool initializeOpenGL(SDL_Window*& window, SDL_GLContext& context) {
    // Initialize OpenGL context
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::cerr << "SDL_Init Error: " << SDL_GetError() << std::endl;
        return false;
    }

    // Create an SDL window with OpenGL context
    window = SDL_CreateWindow("OpenGL + SDL2 Demo", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_OPENGL);
    if (!window) {
        std::cerr << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
        return false;
    }

    // Create OpenGL context
    context = SDL_GL_CreateContext(window);
    if (!context) {
        std::cerr << "SDL_GL_CreateContext Error: " << SDL_GetError() << std::endl;
        return false;
    }

    // Initialize GLEW
    glewInit();

    // Set the OpenGL viewport
    glViewport(0, 0, 800, 600);

    return true;
}
struct color
{
    uint8_t r;
    uint8_t g;
    uint8_t b;
};

uint64_t num_ticks = 0;



int main() {

    entt::entity t;
    entt::registry reg;
    Window window = {300, 300};

    SDL_Event event;
    while (window.is_running()) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                window.quit();
            }
        }


        // Swap buffers to display the rendered content
        glBindFramebuffer(GL_FRAMEBUFFER, window.getFramebuffer());
        //window.swapBuffers();
        num_ticks++;
    }
    


    return 0;
}
