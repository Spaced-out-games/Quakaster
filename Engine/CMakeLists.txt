# Quakaster/Engine/CMakeLists.txt
# Collect engine sources (add files here as you create them)
set(ENGINE_SOURCES
    # engine core
    core/core/Application.cpp
    core/core/Quakaster.cpp
    core/core/Window.cpp

    # engine io
    core/io/DefaultEvents.cpp
    core/io/Event.cpp
    core/io/EventQueue.cpp
    core/io/Layer.cpp
    core/io/LayerStack.cpp

    # glfw
    core/platform/glfw_EventTranslator.cpp

    # resources
    core/res/Image.cpp

    # utilities
    core/utility/ScopeTimer.cpp
    core/utility/Service.cpp
    core/utility/Stopwatch.cpp
    core/utility/time.cpp

    core.cpp
)

message(STATUS "[Engine] sources: ${ENGINE_SOURCES}")

# If no sources exist yet, CMake needs at least a placeholder;
# remove this when real sources are added.
if(NOT ENGINE_SOURCES)
    message(STATUS "[Engine] No source files found — target is interface-only until sources are added.")
    add_library(Engine INTERFACE)
    target_include_directories(Engine INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
    return()
endif()

# ── Library target ────────────────────────────────────────────────────────────
add_library(Engine ${ENGINE_SOURCES} )   # type driven by BUILD_SHARED_LIBS

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