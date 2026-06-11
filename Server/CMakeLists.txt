# Quakaster/Client/CMakeLists.txt
cmake_minimum_required(VERSION 3.16)
project(Server)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SERVER_SOURCES 
	ServerApp.cpp

)
message("[SERVER] Building target `Server`...")
add_executable(${PROJECT_NAME} ${SERVER_SOURCES})
target_link_libraries(${PROJECT_NAME} PRIVATE qk_core)
target_link_libraries(${PROJECT_NAME} PRIVATE Engine)


