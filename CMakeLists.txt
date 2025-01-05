cmake_minimum_required(VERSION 3.17)
project(Engine_Demo VERSION 1.0 LANGUAGES CXX)





set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_PREFIX_PATH "")

# Set default build type if not specified
#if(NOT CMAKE_BUILD_TYPE)

#endif()

# Undefine _DEBUG to ensure it's not defined in the release build
if(CMAKE_BUILD_TYPE STREQUAL "Release")
    add_definitions(-U_DEBUG)
endif()

# Define paths for dependencies
set(SDL_ROOT "D:/C++ Libraries/SDL2")
set(GLEW_ROOT "D:/C++ Libraries/glew-2.2.0")
set(GLM_ROOT "D:/C++ Libraries/glm")
set(IMGUI_ROOT "D:/C++ Libraries/imgui")
set(SDL2_DLL ${SDL_ROOT}/lib/x64/SDL2.dll)

# Include Directories
include_directories(
    ${CMAKE_SOURCE_DIR}
    ${SDL_ROOT}/include
    ${GLEW_ROOT}/include
    ${GLM_ROOT}/include
    ${IMGUI_ROOT}
    ${IMGUI_ROOT}/backends
)

# Link Directories
link_directories(
    ${SDL_ROOT}/lib/x64
    ${GLEW_ROOT}/lib/Release/x64
)

# Add definitions
add_definitions(-DSDL_MAIN_HANDLED -DGLEW_STATIC)

# Find OpenGL
find_package(OpenGL REQUIRED)

# Source Files
file(GLOB SOURCES ${CMAKE_SOURCE_DIR}/src/*.cpp)

# Add ImGui sources
file(GLOB IMGUI_SOURCES
    ${IMGUI_ROOT}/*.cpp
    ${IMGUI_ROOT}/backends/imgui_impl_sdl2.cpp
    ${IMGUI_ROOT}/backends/imgui_impl_opengl3.cpp
)
list(APPEND SOURCES ${IMGUI_SOURCES})

# Define executable target
add_executable(Engine_Demo ${SOURCES} 
    "include/GameContext/GameContext.h"
    "include/GameContext/client/application.h"
    "include/GameContext/Window/window.h"
    "include/GameContext/server/server.h"
    "include/GameContext/server/interpreter/ConsoleInterpreter.h"
    "include/GameContext/Scene.h"
    "include/GameContext/Window/Renderer.h"
    "include/GameContext/client/Client.h"
    "include/GameContext/UI/UIContext.h"
    "include/GameContext/UI/ConsoleUI.h"
    "include/GameContext/IO/controller.h"
    "include/GameContext/IO/controllerEvents.h"
    "include/GameContext/UI/console_message.h"
    "include/GameContext/IO/InputBase.h"
    "include/GameContext/UI/UIBase.h"
    "include/GameContext/utils/utility_types.h"
    "include/GameContext/entity_components/basic_components.h"
    "include/GameContext/ECS/Entity.h"
    "include/GameContext/utils/Inspector.h"
 "include/GameContext/server/interpreter/InterpreterToken.h" "include/GameContext/server/interpreter/convar.h")

# Link libraries
target_link_libraries(Engine_Demo
    PRIVATE
    ${SDL_ROOT}/lib/x64/SDL2.lib
    ${SDL_ROOT}/lib/x64/SDL2main.lib
    ${OPENGL_gl_LIBRARY}
    ${GLEW_ROOT}/lib/Release/x64/glew32s.lib
)

# Post-build command to copy SDL2.dll
add_custom_command(TARGET Engine_Demo POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    ${SDL2_DLL}
    $<TARGET_FILE_DIR:Engine_Demo>
)
