# Quakaster/Core/CMakeLists.txt
add_library(qk_core INTERFACE)


# exposes the headers
target_include_directories(qk_core INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})