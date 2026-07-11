add_library(qk_core INTERFACE)

target_include_directories(qk_core INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(qk_core INTERFACE
    spdlog::spdlog
)