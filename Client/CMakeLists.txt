file(GLOB_RECURSE CLIENT_SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/*.cxx"
)

if(NOT CLIENT_SOURCES)
    message(STATUS "[Client] No source files found — skipping executable target.")
    return()
endif()

add_executable(Client ${CLIENT_SOURCES})

target_link_libraries(Client PRIVATE Engine)
