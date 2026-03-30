file(GLOB_RECURSE SERVER_SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/*.cxx"
)

if(NOT SERVER_SOURCES)
    message(STATUS "[Server] No source files found — skipping executable target.")
    return()
endif()

add_executable(Server ${SERVER_SOURCES})

target_link_libraries(Server PRIVATE Engine)