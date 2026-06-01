# Quakaster/Client/CMakeLists.txt
cmake_minimum_required(VERSION 3.16)
project(Client)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CLIENT_SOURCES 
	ClientApplication.cpp

)
message("[CLIENT] Building target `Client`...")
add_executable(${PROJECT_NAME} ${CLIENT_SOURCES} "ClientApplication.cpp")
target_link_libraries(${PROJECT_NAME} PRIVATE qk_core)
target_link_libraries(${PROJECT_NAME} PRIVATE Engine)


