cmake_minimum_required(VERSION 3.17)
project(Engine_Demo VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_PREFIX_PATH "")

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


#find_package(assimp CONFIG REQUIRED)
#target_link_libraries(main PRIVATE assimp::assimp)
# check here:
# C:/Users/devin/vcpkg/packages/assimp_x86-windows/share/assimp/copyright
# -DCMAKE_TOOLCHAIN_FILE=C:/Users/devin/vcpkg/scripts/buildsystems/vcpkg.cmake
#set(CMAKE_TOOLCHAIN_FILE "C:/Users/devin/vcpkg/scripts/buildsystems/vcpkg.cmake" CACHE STRING "Vcpkg toolchain file")


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
add_executable(Engine_Demo ${SOURCES}  "include/entity.h" "include/scene.h" "include/think.h" "include/utility_types.h" "include/textComponent.h" "include/consoleComponent.h" )

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