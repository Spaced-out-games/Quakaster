cmake_minimum_required(VERSION 3.17)
project(MyProject VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)


#These need to be fixed to use a PATH variable
# Add GLEW root folder
set(LIBRARY_DIRECTORY "D:/C++ Libraries/")
#set(ENTT_ROOT_FOLDER "D:/C++ Libraries/entt")
#set(GLM_ROOT_FOLDER "D:/C++ Libraries/glm")


# Preprocessor definitions
add_definitions(-DSDL_MAIN_HANDLED -DSDL_STATIC -DGLEW_STATIC)

# Include Directories
include_directories(
    ${CMAKE_SOURCE_DIR}
    "$ENV{SDL_ROOT_FOLDER}/include"
    "$ENV{GLEW_ROOT_FOLDER}/include"
    "$ENV{ENTT_ROOT_FOLDER}/single_include"
    "$ENV{GLM_ROOT_FOLDER}/include"
    "${LIBRARY_DIRECTORY}"


)

# Link Directories
link_directories(
    "$ENV{SDL_ROOT_FOLDER}/lib/x64"
    "$ENV{GLEW_ROOT_FOLDER}/lib/Release/x64"
)

# Find packages
find_package(OpenGL REQUIRED)
find_package(SDL2 REQUIRED)

# Source Files
file(GLOB SOURCES ${CMAKE_SOURCE_DIR}/src/*.cpp)

# Define executable target
add_executable(MyProject ${SOURCES})

# Link libraries
target_link_libraries(MyProject
    PRIVATE
    SDL2::SDL2
    SDL2::SDL2main
    OpenGL::GL
    glew32s  # For static linking
)
