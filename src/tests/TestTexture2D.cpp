#include "Test.h"
#include "TestTexture2D.h"


#include "../Renderer.h"
#include "../include/imgui/imgui.h"

#include <iostream>


namespace test {
    
    TestTexture2D::TestTexture2D() : m_TranslationA(200, 200, 0), m_TranslationB(400, 200, 0),
        m_Proj(glm::ortho(0.0f, 960.0f, 0.0f, 540.0f, -1.0f, 1.0f)), m_View(glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0)))
    {
        float positions[] = {
            -50.0f, -50.0f, 0.0f, 0.0f,
             50.0f, -50.0f, 1.0f, 0.0f,
             50.0f,  50.0f, 1.0f, 1.0f,
            -50.0f,  50.0f, 0.0f, 1.0f
        };

        unsigned int indices[] = {
            0, 1, 2,
            2, 3, 0
        };

        GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA)); // Set the blending function
        GLCall(glEnable(GL_BLEND)); // Enable blending

        m_VAO = std::make_unique<VertexArray>(); // Instantiate a vertex array

        m_VertexBuffer = std::make_unique<VertexBuffer>(positions, 4 * 4 * sizeof(float)); // Instantiate a buffer with the positions and binds it by default
        VertexBufferLayout layout; // Instantiate a vertex buffer layout
        layout.Push<float>(2); // Push the positions to the layout
        layout.Push<float>(2); // Push the texture positions to the layout

        m_VAO->addBuffer(*m_VertexBuffer, layout); // Add the buffer to the vertex array
        m_IndexBuffer = std::make_unique<IndexBuffer>(indices, 6); // Instantiate an index buffer

        m_Shader = std::make_unique<Shader>("res/shaders/basic.shader");
        m_Shader->Bind(); // Select the program shader
        m_Shader->SetUniform4f("u_Color", 0.8f, 0.3, 0.8f, 1.0f); // Set the uniform 4f (four floats)
        m_Texture = std::make_unique<Texture>("res/textures/ChernoLogo.png"); // Instantiate a texture
        m_Shader->SetUniform1i("u_Texture", 0); // That's how we select textures in the shader
    }

    TestTexture2D::~TestTexture2D()
    {
    }

    void TestTexture2D::OnUpdate(float deltaTime)
    {
    }

    void TestTexture2D::OnRender()
    {
        GLCall(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
        GLCall(glClear(GL_COLOR_BUFFER_BIT));

        m_Texture->Bind(); // Bind the texture
        Renderer renderer;
        
        {   
            glm::mat4 model = glm::translate(glm::mat4(1.0f), m_TranslationA);
            glm::mat4 mvp = m_Proj * m_View * model;
            m_Shader->Bind();
            m_Shader->SetUniformMat4f("u_MVP", mvp);
            renderer.Draw(*m_VAO, *m_IndexBuffer, *m_Shader);
        }

        {
            glm::mat4 model = glm::translate(glm::mat4(1.0f), m_TranslationB);
            glm::mat4 mvp = m_Proj * m_View * model;
            m_Shader->Bind();
            m_Shader->SetUniformMat4f("u_MVP", mvp);
            renderer.Draw(*m_VAO, *m_IndexBuffer, *m_Shader);
        }

    }

    void TestTexture2D::OnImGuiRender()
    {
        ImGui::SliderFloat3("translationA", &m_TranslationA.x, 0.0f, 960.0f);
        ImGui::SliderFloat3("translationB", &m_TranslationB.x, 0.0f, 960.0f);
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    }
}