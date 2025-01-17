#pragma once
#include <SDL.h>
#include <GL/glew.h>
#include <include/thirdparty/glm/glm.hpp>
#include <imgui.h>
#include <backends/imgui_impl_sdl2.h>
#include <backends/imgui_impl_opengl3.h>
#include <include/GameContext/window/Renderer.h>

//#define WINDOW_WIDTH 1366
//#define WINDOW_HEIGHT 768

#define WINDOW_WIDTH 1920
#define WINDOW_HEIGHT 1080

#include <include/GameContext/graphics/VAO.h>


class Window {
public:
    Window(ConsoleInterpreter& interpreter, int width, int height);
    ~Window();

    void swapBuffers();
    void resize(int new_width, int new_height);
    bool is_running() const { return running; }
    void quit() { running = false; }
    SDL_Window* getWindow() const { return sdl_window; }
    SDL_GLContext getContext() const { return renderer.gl_context; }

    void beginImGuiFrame();
    void endImGuiFrame();
    void renderImGui();
    void cleanupImGui();
    void renderToTexture();
    GLuint renderTexture = 0;

    inline Renderer& get_renderer() { return renderer; }
    SDL_Window* sdl_window = nullptr;

private:
    void createFramebuffer();
    Renderer renderer;
    //SDL_GLContext context = nullptr;
    glm::ivec2 dimensions;
    // ImGuiContext* ImGUIContext = nullptr;



    // ImGuiIO* ImGUI_IO = nullptr;
    bool running = true;
};


Window::Window(ConsoleInterpreter& interpreter, int width = WINDOW_WIDTH, int height = WINDOW_HEIGHT): renderer(interpreter) {
    dimensions = { width, height };

    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::cerr << "SDL_Init Error: " << SDL_GetError() << std::endl;
        exit(1);
    }

    // Set SDL OpenGL attributes
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);

    // Create SDL window
    sdl_window = SDL_CreateWindow("OpenGL + SDL2 FBO Demo", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
    if (!sdl_window) {
        std::cerr << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
        SDL_Quit(); // Cleanup SDL before exit
        exit(2);
    }

    // Create OpenGL context
    renderer.gl_context = SDL_GL_CreateContext(sdl_window);
    if (!renderer.gl_context) {
        std::cerr << "SDL_GL_CreateContext Error: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(sdl_window); // Cleanup the window before exit
        SDL_Quit();
        exit(3);
    }

    // Initialize GLEW
    GLenum err = glewInit();
    if (err != GLEW_OK) {
        std::cerr << "GLEW Error: " << glewGetErrorString(err) << std::endl;
        SDL_GL_DeleteContext(renderer.gl_context); // Cleanup context before exit
        SDL_DestroyWindow(sdl_window);
        SDL_Quit();
        exit(4);
    }

    // Set the OpenGL viewport
    glViewport(0, 0, dimensions.x, dimensions.y);

    // Create the framebuffer
    createFramebuffer();

    // Initialize ImGui
    IMGUI_CHECKVERSION();
    renderer.gui_context = ImGui::CreateContext();
    ImGui::StyleColorsDark();

    // Initialize ImGui backends
    if (!ImGui_ImplSDL2_InitForOpenGL(sdl_window, renderer.gl_context)) {
        std::cerr << "Failed to initialize ImGui SDL backend" << std::endl;
        ImGui::DestroyContext(renderer.gui_context); // Cleanup ImGui context before exit
        SDL_GL_DeleteContext(renderer.gl_context);
        SDL_DestroyWindow(sdl_window);
        SDL_Quit();
        exit(5);
    }
    if (!ImGui_ImplOpenGL3_Init("#version 330")) {
        std::cerr << "Failed to initialize ImGui OpenGL backend" << std::endl;
        ImGui_ImplSDL2_Shutdown(); // Clean up before exit
        ImGui::DestroyContext(renderer.gui_context);
        SDL_GL_DeleteContext(renderer.gl_context);
        SDL_DestroyWindow(sdl_window);
        SDL_Quit();
        exit(6);
    }


    if (SDL_SetRelativeMouseMode(SDL_TRUE) != 0) {
        SDL_Log("Failed to enable relative mouse mode: %s", SDL_GetError());
    }
    // Optional: Uncomment if needed
    // renderer.gui_context->ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable keyboard controls

    // Bind viewport if necessary
    // viewport.bind(); // Ensure this is defined and necessary

    renderer.init();
}

Window::~Window() {
    // Cleanup ImGui
    cleanupImGui();

    // Cleanup OpenGL objects

    //viewport.~FrameBuffer();

    SDL_GL_DeleteContext(renderer.gl_context);
    SDL_DestroyWindow(sdl_window);
    SDL_Quit();
}

void Window::swapBuffers() {
    SDL_GL_SwapWindow(sdl_window);
}

void Window::resize(int new_width, int new_height) {
    dimensions = { new_width, new_height };
    glViewport(0, 0, dimensions.x, dimensions.y);

    // Recreate the framebuffer for the new dimensions
    createFramebuffer();
}

void Window::createFramebuffer() {
    // Clean up any existing FBO
    //viewport.setup((unsigned int)dimensions.x, (unsigned int)dimensions.y);
}

void Window::beginImGuiFrame() {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();
}

void Window::endImGuiFrame() {
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    swapBuffers();

}

void Window::renderImGui() {
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void Window::cleanupImGui() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();
}

void Window::renderToTexture() {

}