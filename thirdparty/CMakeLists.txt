# Quakaster/thirdparty/CMakeLists.txt

# ImGui 
add_library(imgui STATIC
    imgui/imgui.cpp
    imgui/imgui_draw.cpp
    imgui/imgui_tables.cpp
    imgui/imgui_widgets.cpp
    imgui/backends/imgui_impl_glfw.cpp
    imgui/backends/imgui_impl_opengl3.cpp  # swap for your backend
)
target_include_directories(imgui PUBLIC imgui imgui/backends)
target_link_libraries(imgui PUBLIC glfw)

# Boost.PFR (header-only) 
add_library(pfr INTERFACE)
target_include_directories(pfr INTERFACE pfr/include)

# stb_image (header-only, needs one .cpp to house the implementation)
add_library(stb_image STATIC "stb/stb_image.cpp")
target_include_directories(stb_image PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})


