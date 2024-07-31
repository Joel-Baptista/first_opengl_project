cmake_minimum_required(VERSION 3.11)
project(game)

find_package(glfw3 REQUIRED)
find_package(GLEW REQUIRED)

set(CMAKE_CXX_STANDARD 17)

find_package(OpenGL REQUIRED)

add_executable(game
    src/main.cpp
    src/IndexBuffer.cpp
    src/Renderer.cpp
    src/VertexBuffer.cpp   
    src/VertexArray.cpp
    src/Shader.cpp
    src/include/stb_image/stb_image.cpp
    src/Texture.cpp
)

target_link_libraries(game PRIVATE glfw GLEW OpenGL::GL)

enable_testing()

