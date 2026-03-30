# Quakaster/Engine/CMakeLists.txt
# Collect engine sources (add files here as you create them)
file(GLOB_RECURSE ENGINE_SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/*.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/*.cxx"
)

# If no sources exist yet, CMake needs at least a placeholder;
# remove this when real sources are added.
if(NOT ENGINE_SOURCES)
    message(STATUS "[Engine] No source files found — target is interface-only until sources are added.")
    add_library(Engine INTERFACE)
    target_include_directories(Engine INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
    return()
endif()

# ── Library target ────────────────────────────────────────────────────────────
add_library(Engine ${ENGINE_SOURCES})   # type driven by BUILD_SHARED_LIBS

target_include_directories(Engine
    PUBLIC  ${CMAKE_CURRENT_SOURCE_DIR}
)

# ── QK_STATIC / QK_SHARED define ─────────────────────────────────────────────
if(BUILD_SHARED_LIBS)
    target_compile_definitions(Engine PRIVATE QK_EXPORT PUBLIC QK_SHARED)
else()
    target_compile_definitions(Engine PUBLIC QK_STATIC)
endif()

target_link_libraries(Engine
    PUBLIC
        EnTT::EnTT
        glm::glm-header-only
        spdlog::spdlog
        imgui
        pfr
        stb_image
    PRIVATE
        glfw
        enet
)