


cmake_minimum_required(VERSION 3.16)
project(qk_core)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CORE_SOURCES
    "Resource.cpp"
   "Partition.cpp")

add_library(qk_core ${CORE_SOURCES})

target_include_directories(qk_core PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(qk_core
    PUBLIC
        spdlog::spdlog
)