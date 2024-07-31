#include <GL/glew.h>
#include <GL/glut.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <sstream>

#include "Renderer.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"  
#include "Shader.h"
#include "VertexBufferLayout.h"
#include "Texture.h"

#include "include/glm/glm.hpp"
#include "include/glm/gtc/matrix_transform.hpp"


int main(void)
{   
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // Set the major version of OpenGL
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); // Set the minor version of OpenGL
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // Set the profile of OpenGL to core (No backwards compatibility)

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    glfwSwapInterval(1); // Enable VSync

    // Initialize GLEW only after creating the window
    if (glewInit() != GLEW_OK) 
        std::cout << "Error!" << std::endl;

    std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;

    float positions[] = {
        -0.5f, -0.5f, 0.0f, 0.0f,
         0.5f, -0.5f, 1.0f, 0.0f,
         0.5f,  0.5f, 1.0f, 1.0f,
        -0.5f,  0.5f, 0.0f, 1.0f
    };

    unsigned int indices[] = {
        0, 1, 2,
        2, 3, 0
    };

    GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA)); // Set the blending function
    GLCall(glEnable(GL_BLEND)); // Enable blending

    VertexArray va; // Instantiate a vertex array
    VertexBuffer vb(positions, 4 * 4 * sizeof(float)); // Instantiate a buffer with the positions and binds it by default
    VertexBufferLayout layout; // Instantiate a vertex buffer layout
    layout.Push<float>(2); // Push the positions to the layout
    layout.Push<float>(2); // Push the texture positions to the layout
    va.addBuffer(vb, layout); // Add the buffer to the vertex array

    IndexBuffer ib(indices, 6); // Instantiate an index buffer

    glm::mat4 proj = glm::ortho(-1.0f, 1.0f, -0.75f, 0.75f, -1.0f, 1.0f); // Create an orthographic projection matrix by giving it the aspect ratio 4x3 of our image
    
    Shader shader("res/shaders/basic.shader");
    shader.Bind(); // Select the program shader
    shader.SetUniform4f("u_Color", 0.8f, 0.3, 0.8f, 1.0f); // Set the uniform 4f (four floats)
    shader.SetUniformMat4f("u_MVP", proj); // Set the uniform matrix 4f (four floats)

    Texture texture("res/textures/ChernoLogo.png");
    texture.Bind(); // Select the texture
    shader.SetUniform1i("u_Texture", 0); // That's how we select textures in the shader

    // Clear OpenGL states
    va.Unbind(); // Select no vertex array
    shader.Unbind(); // Select no program
    vb.Unbind(); // Select no buffer
    ib.Unbind(); // Select no element buffer

    Renderer renderer; // Instantiate a renderer

    float r = 0.0f;
    float increment = 0.05f;
    // glBindBuffer(GL_ARRAY_BUFFER, 0); // Select no buffer
    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        renderer.Clear(); // Clear the color buffer

        shader.Bind(); // Use the program
        shader.SetUniform4f("u_Color", r, 0.3, 0.8f, 1.0f); // Set the uniform 4f (four floats)

        renderer.Draw(va, ib, shader); // Draw the vertex array, index buffer and shader

        if (r > 1.0f){
            r = 1.0f;
            increment = -0.05f;
        }else if (r < 0.0f){
            r = 0.0f;
            increment = 0.05f;
        }
        r += increment;

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}


