#include "include/window.h"


#define WINDOW_WIDTH 1920
#define WINDOW_HEIGHT 1080

Window::Window(): Window (WINDOW_WIDTH, WINDOW_HEIGHT) {}

Window::Window(int width, int height) {
    dimensions = { width, height };

    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::cerr << "SDL_Init Error: " << SDL_GetError() << std::endl;
        exit(1);
    }

    // Set SDL OpenGL attributes
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    // Create SDL window
    window = SDL_CreateWindow("OpenGL + SDL2 FBO Demo", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
    if (!window) {
        std::cerr << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
        exit(2);
    }

    // Create OpenGL context
    context = SDL_GL_CreateContext(window);
    if (!context) {
        std::cerr << "SDL_GL_CreateContext Error: " << SDL_GetError() << std::endl;
        exit(3);
    }

    // Initialize GLEW
    GLenum err = glewInit();
    if (err != GLEW_OK) {
        std::cerr << "GLEW Error: " << glewGetErrorString(err) << std::endl;
        exit(4);
    }

    // Set the OpenGL viewport
    glViewport(0, 0, dimensions.x, dimensions.y);

    // Create the framebuffer
    createFramebuffer();

    // Initialize ImGui
    IMGUI_CHECKVERSION();
    ImGUIContext = ImGui::CreateContext();
    ImGUI_IO = &(ImGui::GetIO());
    ImGUI_IO->ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Optional: Enable keyboard controls
    ImGui::StyleColorsDark();

    // Initialize ImGui backends
    if (!ImGui_ImplSDL2_InitForOpenGL(window, context)) {
        std::cerr << "Failed to initialize ImGui SDL backend" << std::endl;
        exit(5);
    }
    if (!ImGui_ImplOpenGL3_Init("#version 330")) {
        std::cerr << "Failed to initialize ImGui OpenGL backend" << std::endl;
        exit(6);
    }
    //viewport.bind();
}

Window::~Window() {
    // Cleanup ImGui
    cleanupImGui();

    // Cleanup OpenGL objects
    
    //viewport.~FrameBuffer();

    SDL_GL_DeleteContext(context);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void Window::swapBuffers() {
    SDL_GL_SwapWindow(window);
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