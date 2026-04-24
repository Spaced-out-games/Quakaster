add_definitions(-DGLEW_STATIC)
find_package(OpenGL REQUIRED)

# ImGui core
add_library(imgui_core STATIC
    imgui/imgui.cpp
    imgui/imgui_draw.cpp
    imgui/imgui_tables.cpp
    imgui/imgui_widgets.cpp
)

target_include_directories(imgui_core PUBLIC imgui)

# ImGui OpenGL backend
add_library(imgui_backend_gl STATIC
    imgui/backends/imgui_impl_glfw.cpp
    imgui/backends/imgui_impl_opengl3.cpp
)

target_include_directories(imgui_backend_gl
    PUBLIC imgui
    PRIVATE imgui/backends
)

find_package(GLEW REQUIRED)

target_link_libraries(imgui_backend_gl
    PUBLIC
        imgui_core
        glfw
        GLEW::GLEW
        OpenGL::GL
)

# Boost.PFR
add_library(pfr INTERFACE)
target_include_directories(pfr INTERFACE pfr/include)

# stb_image
add_library(stb_image STATIC stb/stb_image.cpp)
target_include_directories(stb_image PUBLIC stb)