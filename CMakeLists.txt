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
    src/Texture.cpp
    src/tests/TestClearColor.cpp
    src/tests/TestTexture2D.cpp
    src/tests/TestBatchRendering2D.cpp
    src/tests/TestBatchColor.cpp
    src/tests/TestBatchTexture.cpp
    src/tests/Test.cpp
    src/include/stb_image/stb_image.cpp
    src/include/imgui/imgui_impl_glfw_gl3.cpp
    src/include/imgui/imgui_demo.cpp
    src/include/imgui/imgui_draw.cpp
    src/include/imgui/imgui.cpp
)

target_link_libraries(game PRIVATE glfw GLEW OpenGL::GL)

enable_testing()

