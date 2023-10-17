cmake_minimum_required(VERSION 3.22)

project(OpenGL-Demo
	VERSION 0.0
	LANGUAGES C CXX
)

set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/modules/")

add_subdirectory(lib)

find_package(GLFW3 REQUIRED)
find_package(PNG REQUIRED)

add_executable(opengl-demo
	src/main.cpp
	src/glfw-adapters.cpp
	src/simple.cpp
	src/texture.cpp
	src/png-adapters.cpp
)
target_link_libraries(opengl-demo PRIVATE glad glfw png)

file(CREATE_LINK ${CMAKE_CURRENT_SOURCE_DIR}/res ${CMAKE_CURRENT_BINARY_DIR}/res SYMBOLIC)
