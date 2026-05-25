cmake_minimum_required(VERSION 3.10)
project(qk_rhi)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ------------------------------------------------------------------------------
# Select which rendering backend to build.
# This is a CACHE variable so it can be overridden from the command line:
#   cmake -DGFX_BACKEND=vk ..
# ------------------------------------------------------------------------------
set(GFX_BACKEND "gl" CACHE STRING "GFX backend (e.g. gl, vk)")

# ------------------------------------------------------------------------------
# List of source files for the RHI implementation.
# This will be populated depending on which backend is selected.
# ------------------------------------------------------------------------------
set(RHI_SOURCES
    external/imgui/imgui.cpp
    external/imgui/imgui_demo.cpp
    external/imgui/imgui_draw.cpp
    external/imgui/imgui_tables.cpp
    external/imgui/imgui_widgets.cpp
    
    
    gl/gfx_rhi.cpp
    gl/gui_rhi_glfw_impl.cpp
    gl/gui_widget_input.cpp
    gl/gui_widget_rhi.cpp
    gl/HVertexArray.cpp

    
    
    
    
    
    
    
    
    
    
    
  "gl/HBuffer.cpp"
  "gl/HVertexLayout.cpp"


 "gl/Draw.cpp" "gl/HTexture.cpp" "gl/HDescriptorSet.cpp" "gl/HMaterial.cpp" "gl/HCommandBuffer.cpp")

# ------------------------------------------------------------------------------
# Backend selection
# Each backend:
#   - Adds its implementation source files
#   - Finds required packages
#   - (later) defines include paths + link libraries
# ------------------------------------------------------------------------------
if(GFX_BACKEND STREQUAL "gl")
    # OpenGL backend requires system OpenGL
    find_package(OpenGL REQUIRED)

    # Add OpenGL implementation + GLAD loader
    # GLAD is compiled directly into this library (no external dependency leak)
    list(APPEND RHI_SOURCES
        #gl/gfx_rhi.cpp
        external/glad/src/glad.c
        external/imgui/backends/imgui_impl_opengl3.cpp
        external/imgui/backends/imgui_impl_glfw.cpp

    )

elseif(GFX_BACKEND STREQUAL "vk")
    # Vulkan backend not implemented yet
    message(FATAL_ERROR "Vulkan backend not implemented yet")

else()
    # Catch invalid backend values early (fail fast)
    message(FATAL_ERROR "Unknown gfx backend `${GFX_BACKEND}`")
endif()

# ------------------------------------------------------------------------------
# Define the RHI static library
# This is the module the engine links against
# ------------------------------------------------------------------------------
add_library(${PROJECT_NAME} STATIC ${RHI_SOURCES} "gl/HShader.cpp")

# ------------------------------------------------------------------------------
# PUBLIC include directory:
# Only expose the stable RHI API to consumers (Engine, Client, etc.)
# This should contain ONLY backend-agnostic headers.
# ------------------------------------------------------------------------------
target_include_directories(${PROJECT_NAME}
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# ------------------------------------------------------------------------------
# PRIVATE include directories:
# Backend-specific and third-party headers are hidden from consumers.
# This enforces the abstraction boundary.
# ------------------------------------------------------------------------------
if(GFX_BACKEND STREQUAL "gl")
    target_include_directories(${PROJECT_NAME}
        PRIVATE
            external/glad/include
            external/imgui
    )
endif()

# ------------------------------------------------------------------------------
# Link dependencies
#
# PUBLIC:
#   - Propagated to anything that links qk_rhi
#   - qk_core is part of the shared engine foundation
#
# PRIVATE:
#   - Used internally only (not visible to Engine)
#   - Prevents leaking OpenGL into higher layers
# ------------------------------------------------------------------------------
target_link_libraries(${PROJECT_NAME}
    PUBLIC
        qk_core
)

if(GFX_BACKEND STREQUAL "gl")
    target_link_libraries(${PROJECT_NAME}
        PRIVATE
            OpenGL::GL
            glfw
    )
endif()