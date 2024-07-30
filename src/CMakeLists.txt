cmake_minimum_required(VERSION 3.11)
project(game)

find_package(glfw3 REQUIRED)
find_package(GLEW REQUIRED)

set(CMAKE_CXX_STANDARD 17)

find_package(OpenGL REQUIRED)

add_executable(game
    main.cpp
)

target_link_libraries(game PRIVATE glfw GLEW OpenGL::GL)

enable_testing()

