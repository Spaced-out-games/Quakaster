# Quakaster/Engine/CMakeLists.txt
cmake_minimum_required(VERSION 3.16)
project(Engine)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(ENGINE_SOURCES
	platform/Application.cpp
    platform/Quakaster.cpp
    platform/Window.h
    
    io/DefaultEvents.cpp
    io/Event.cpp
    io/EventQueue.cpp
    io/Layer.cpp
    io/LayerStack.cpp
    utility/time.cpp



    # Platform-specific files
    platform/Window_glfw_impl.cpp

    media/Image.cpp
    ../vendor/stb_image.cpp
 "gui/GUILayer.cpp")

message("[ENGINE] Building target `Engine`...")
add_library(${PROJECT_NAME} ${ENGINE_SOURCES})

# engine include lists go here
target_link_libraries(${PROJECT_NAME}
    INTERFACE 
    PUBLIC
        qk_core
        qk_rhi
        glm::glm # consider making a wrapper. Probably don't need to, though
    PRIVATE
        spdlog::spdlog
        glfw
)

target_include_directories(Engine
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}

    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/../vendor
)


message(STATUS "[ENGINE] [DEBUG] CMAKE_CURRENT_DIRECTORY: ${CMAKE_CURRENT_SOURCE_DIR}")


#target_include_directories(${PROJECT_NAME}
#    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
#)


target_compile_definitions(${PROJECT_NAME}
    PRIVATE QK_BUILD_ENGINE
)

