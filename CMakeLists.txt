# Minimum required version of CMake
cmake_minimum_required(VERSION 3.17)

# Project name and version
project(MyProject VERSION 1.0 LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Add CMake prefix path for finding libraries (for SDL2, for example)
set(CMAKE_PREFIX_PATH "D:/C++ Libraries/SDL2/SDL2/cmake")

# Define preprocessor flags for static SDL2 linking
add_definitions(-DSDL_MAIN_HANDLED -DSDL_STATIC)

# ---------------------------------------------------------------------------
# Include Directories
# ---------------------------------------------------------------------------

# Include SDL2 and GLEW paths (adjust paths accordingly)
include_directories(
    "D:/C++ Libraries/SDL2/SDL2/include"         # SDL2 include directory
    "D:/C++ Libraries/glew-2.2.0/include"       # GLEW include directory
    ${CMAKE_SOURCE_DIR}/include                 # Include project headers
    ${CMAKE_SOURCE_DIR}/include/thirdparty/entt/single_include  # EnTT
    ${CMAKE_SOURCE_DIR}/include/thirdparty/glm/include/glm
)

# ---------------------------------------------------------------------------
# Link Directories
# ---------------------------------------------------------------------------

# Specify directories containing library files
link_directories(
    "D:/C++ Libraries/SDL2/SDL2/lib/x64"        # SDL2 lib directory
    "D:/C++ Libraries/glew-2.2.0/lib/Release/x64" # GLEW lib directory
)

# ---------------------------------------------------------------------------
# Find and Add Dependencies
# ---------------------------------------------------------------------------

# Find packages
find_package(OpenGL REQUIRED)

# Find SDL2 manually and link statically
find_package(SDL2 REQUIRED)

# Enable Doxygen documentation generation (optional)
find_package(Doxygen REQUIRED)

# ---------------------------------------------------------------------------
# Source Files
# ---------------------------------------------------------------------------

# Glob all source files in src directory
file(GLOB SOURCES ${CMAKE_SOURCE_DIR}/src/*.cpp)

# ---------------------------------------------------------------------------
# Executable Target
# ---------------------------------------------------------------------------

# Define the executable
add_executable(MyProject ${SOURCES} "include/position.h")

# Link libraries (SDL2, GLEW, OpenGL)
# Link to SDL2 static library
target_link_libraries(MyProject 
    PRIVATE 
    SDL2::SDL2
    SDL2::SDL2main
    OpenGL::GL
    glew32s
)

add_custom_command(TARGET MyProject POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    "D:/C++ Libraries/SDL2/SDL2/lib/x64/SDL2.dll"
    $<TARGET_FILE_DIR:MyProject>)

# ---------------------------------------------------------------------------
# Doxygen Documentation (Optional)
# ---------------------------------------------------------------------------

# If Doxygen is found, add documentation generation target
if(DOXYGEN_FOUND)
    set(DOXYGEN_IN ${CMAKE_SOURCE_DIR}/Doxyfile)
    set(DOXYGEN_OUT ${CMAKE_BINARY_DIR}/docs)

    add_custom_target(doc ALL
        COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_IN}
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        COMMENT "Generating API documentation with Doxygen"
        VERBATIM
    )
else()
    message(WARNING "Doxygen not found! Documentation will not be generated.")
endif()
