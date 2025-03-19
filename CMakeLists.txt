cmake_minimum_required(VERSION 3.17)
project(Engine_Demo VERSION 1.0 LANGUAGES CXX)


# custom macros
# add_definitions(-DPRINT_GL_ERROR)



# set the C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_PREFIX_PATH "")

# Undefine _DEBUG to ensure it's not defined in the release build
#if(CMAKE_BUILD_TYPE STREQUAL "Release")
#    add_definitions(-U_DEBUG)
#endif()

# Define paths for dependencies
set(SDL_ROOT "D:/C++ Libraries/SDL2")
set(GLEW_ROOT "D:/C++ Libraries/glew-2.2.0")
set(GLM_ROOT "D:/C++ Libraries/glm")
set(IMGUI_ROOT "D:/C++ Libraries/imgui")


# Once you restart, this should work
# set(SDL_ROOT "$ENV{CPP_LIB}/SDL2")
# set(GLEW_ROOT "$ENV{CPP_LIB}/glew-2.2.0")
# set(GLM_ROOT "$ENV{CPP_LIB}/glm")
# set(IMGUI_ROOT "$ENV{CPP_LIB}/imgui")


set(SDL2_DLL ${SDL_ROOT}/lib/x64/SDL2.dll)

# Include Directories
include_directories(
    ${CMAKE_SOURCE_DIR}
    ${SDL_ROOT}/include
    ${GLEW_ROOT}/include
    ${IMGUI_ROOT}
    ${IMGUI_ROOT}/backends
)

# Link Directories
link_directories(
    ${SDL_ROOT}/lib/x64
    ${GLEW_ROOT}/lib/Release/x64
)

# Add definitions 
add_definitions(-DSDL_MAIN_HANDLED -DGLEW_STATIC -DROOT_DIRECTORY="${CMAKE_SOURCE_DIR}/")


# Find OpenGL
find_package(OpenGL REQUIRED)

# Source Files
# list all directories where *.cpp files are located
file(GLOB_RECURSE SOURCES
    ${CMAKE_SOURCE_DIR}/src/*.cpp
    ${CMAKE_SOURCE_DIR}/include/components/*.cpp
)

# Add ImGui sources
file(GLOB IMGUI_SOURCES
    ${IMGUI_ROOT}/*.cpp
    ${IMGUI_ROOT}/backends/imgui_impl_sdl2.cpp
    ${IMGUI_ROOT}/backends/imgui_impl_opengl3.cpp
)
list(APPEND SOURCES ${IMGUI_SOURCES})

# Specify the root include directory
set(INCLUDE_DIR "${CMAKE_SOURCE_DIR}/include")

# Define executable target
# Use GLOB_RECURSE to find all header files in the include/GameContext directory and its subdirectories
file(GLOB_RECURSE HEADERS
    "${INCLUDE_DIR}/*.h"
)

# Combine source files from GLOB_RECURSE with the gathered headers
add_executable(Engine_Demo ${SOURCES} ${HEADERS}  "include/components/Freecam.h")

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
