# Quakaster/Engine/CMakeLists.txt
cmake_minimum_required(VERSION 3.16)
project(Engine)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(QK_GIT_HASH "unknown")
set(QK_GIT_BRANCH "unknown")
set(QK_GIT_DIRTY "unknown")
set(ENGINE_SOURCES

    # vendor
    ../vendor/stb_image.cpp

    # platform
	platform/Application.cpp
    platform/Quakaster.cpp
    platform/Window.h

    # Platform-specific
    platform/Window_glfw_impl.cpp

    
    # io
    io/DefaultEvents.cpp
    io/Event.cpp
    io/EventQueue.cpp
    io/Layer.cpp
    io/LayerStack.cpp

    # utility
    utility/time.cpp
    utility/ScopeTimer.cpp


    # media
    media/Image.cpp
    media/json.cpp

    # gui
    gui/GUILayer.cpp
    
    # entities

    # components
    components/CCamera.cpp

    # systems
    "kernel/System.cpp"


    # services
    "kernel/IService.cpp"
    
    

    # scripting
    
    

    # gfx
    
    #"net/Endpoint.cpp" "net/Connection.cpp" "net/PacketQueue.cpp"

    "net-v2/Peer.cpp" "net-v2/PeerRegistry.h"

    "utility/ByteStream.cpp"    "media/LoadPipeline.cpp"  "gfx/gfx.cpp"  "math/geometric/Vec3.cpp" "math/geometric/Vec2.cpp" "math/geometric/Vec4.cpp" "math/geometric/Quat.cpp" "spacial/SpatialHashMap.cpp" "math/geometric/Plane.cpp" "gfx/CStaticMeshInstance.cpp" "io/CEventListener.cpp" "math/geometric/Transform.cpp" "math/geometric/Mat4.cpp" "kernel/ComponentLoader.cpp" "kernel/EntityLoader.cpp" "kernel/SceneLoader.cpp"   "gfx/InstancePool.cpp"  "gfx/Renderer.cpp" "debugging/trace.cpp" "utility/Profiler.cpp" "gui/ProfilerWidget.cpp" "../Core/KeyValues.cpp" "kernel/Kernel.cpp" "kernel/VDrive.cpp" "kernel/Port.cpp" "kernel/Signal.cpp" "World/World.cpp" "components/CGraphNode.cpp")

message("[ENGINE] Building target `Engine`...")
add_library(${PROJECT_NAME} ${ENGINE_SOURCES}  "../Core/KeyValues.h")

# engine include lists go here
target_link_libraries(${PROJECT_NAME}
    PUBLIC
        qk_core
        qk_rhi
        glm::glm
        EnTT::EnTT
        nlohmann_json::nlohmann_json
        #spdlog::spdlog
        enet
        #ws2_32 # windows-specific for enet
        #winmm# windows-specific for enet
    PRIVATE
        glfw
)
if(WIN32)
    target_link_libraries(${PROJECT_NAME} PRIVATE ws2_32 winmm)
endif()
       


target_include_directories(Engine
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${enet_SOURCE_DIR}/include

    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/../vendor
)

get_property(_targets DIRECTORY ${enet_SOURCE_DIR} PROPERTY BUILDSYSTEM_TARGETS)
message(STATUS "[ENGINE] [DEBUG] enet_SOURCE_DIR : ${_targets}")
message(STATUS "[ENGINE] [DEBUG] CMAKE_CURRENT_DIRECTORY: ${CMAKE_CURRENT_SOURCE_DIR}")








# Asks Git for some metadata about the project. Used to define some status strings




find_package(Git QUIET)

if(GIT_FOUND)
    execute_process(
        COMMAND "${GIT_EXECUTABLE}" rev-parse --show-toplevel
        WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
        OUTPUT_VARIABLE QK_GIT_ROOT
        OUTPUT_STRIP_TRAILING_WHITESPACE
        ERROR_QUIET
        RESULT_VARIABLE QK_GIT_RESULT
    )

    if(QK_GIT_RESULT EQUAL 0)
        execute_process(
            COMMAND "${GIT_EXECUTABLE}" rev-parse --short HEAD
            WORKING_DIRECTORY "${QK_GIT_ROOT}"
            OUTPUT_VARIABLE QK_GIT_HASH
            OUTPUT_STRIP_TRAILING_WHITESPACE
            ERROR_QUIET
        )

        execute_process(
            COMMAND "${GIT_EXECUTABLE}" branch --show-current
            WORKING_DIRECTORY "${QK_GIT_ROOT}"
            OUTPUT_VARIABLE QK_GIT_BRANCH
            OUTPUT_STRIP_TRAILING_WHITESPACE
            ERROR_QUIET
        )

        execute_process(
            COMMAND "${GIT_EXECUTABLE}" status --porcelain
            WORKING_DIRECTORY "${QK_GIT_ROOT}"
            OUTPUT_VARIABLE QK_GIT_STATUS
            OUTPUT_STRIP_TRAILING_WHITESPACE
            ERROR_QUIET
        )

        if(QK_GIT_STATUS)
            set(QK_GIT_DIRTY "dirty")
        else()
            set(QK_GIT_DIRTY "clean")
        endif()
    endif()
endif()




target_compile_definitions(${PROJECT_NAME}
    PRIVATE
        QK_BUILD_ENGINE
        NOMINMAX
    PUBLIC
        QK_GIT_BRANCH="${QK_GIT_BRANCH}"
        QK_GIT_DIRTY="${QK_GIT_DIRTY}"
        QK_GIT_HASH="${QK_GIT_HASH}"
        #QK_GIT_STATUS="${QK_GIT_STATUS}"
)
