#include "Test.h"
#include "TestBatchTexture.h"


#include "../Renderer.h"
#include "../include/imgui/imgui.h"

#include <iostream>


namespace test {
    
    TestBatchTexture::TestBatchTexture() : m_TranslationA(200, 200, 0), m_TranslationB(500, 200, 0),
        m_Proj(glm::ortho(0.0f, 960.0f, 0.0f, 540.0f, -1.0f, 1.0f)), m_View(glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0)))
    {
        float vertices[] = {
            -50.0f, -50.0f, 0.396f, 0.263f, 0.129f, 1.0f, 0.0f, 0.0f, 0.0f,
             50.0f, -50.0f, 0.396f, 0.263f, 0.129f, 1.0f, 1.0f, 0.0f, 0.0f, 
             50.0f,  50.0f, 0.396f, 0.263f, 0.129f, 1.0f, 1.0f, 1.0f, 0.0f, 
            -50.0f,  50.0f, 0.396f, 0.263f, 0.129f, 1.0f, 0.0f, 1.0f, 0.0f, 

            150.0f, -50.0f, 0.71f, 0.40, 0.11f, 1.0f, 0.0f, 0.0f, 2.0f,
            250.0f, -50.0f, 0.71f, 0.40, 0.11f, 1.0f, 1.0f, 0.0f, 2.0f,
            250.0f,  50.0f, 0.71f, 0.40, 0.11f, 1.0f, 1.0f, 1.0f, 2.0f,
            150.0f,  50.0f, 0.71f, 0.40, 0.11f, 1.0f, 0.0f, 1.0f, 2.0f,
        };

        unsigned int indices[] = {
            0, 1, 2,
            2, 3, 0,
            
            4, 5, 6,
            6, 7, 4,
        };

        GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA)); // Set the blending function
        GLCall(glEnable(GL_BLEND)); // Enable blending

        m_VAO = std::make_unique<VertexArray>(); // Instantiate a vertex array

        m_VertexBuffer = std::make_unique<VertexBuffer>(vertices, 9 * 8 * sizeof(GLfloat)); // Instantiate a buffer with the positions and binds it by default
        VertexBufferLayout layout; // Instantiate a vertex buffer layout
        layout.Push<float>(2); // Push the positions to the layout
        layout.Push<float>(4); // Push the texture positions to the layout
        layout.Push<float>(2); // Push the texture positions to the layout
        layout.Push<float>(1); // Push the texture positions to the layout

        m_VAO->addBuffer(*m_VertexBuffer, layout); // Add the buffer to the vertex array
        m_IndexBuffer = std::make_unique<IndexBuffer>(indices, 12); // Instantiate an index buffer

        m_Shader = std::make_unique<Shader>("res/shaders/batch.shader");
        m_Shader->Bind(); // Select the program shader
        
        m_Textures.push_back(std::make_unique<Texture>("res/textures/ChernoLogo.png")); // Instantiate a texture
        m_Textures.push_back(std::make_unique<Texture>("res/textures/linux.png")); // Instantiate a texture
        
        int sampler[m_Textures.size()];
        
        for (int i = 0; i < m_Textures.size(); i++){
            glBindTextureUnit(i, m_Textures[i]->GetRendererID());  
            sampler[i] = i;
        }

        m_Shader->SetUniform1iv("u_Textures", sampler); // That's how we select textures in the shader
    }

    TestBatchTexture::~TestBatchTexture()
    {
    }

    void TestBatchTexture::OnUpdate(float deltaTime)
    {
    }

    void TestBatchTexture::OnRender()
    {
        GLCall(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
        GLCall(glClear(GL_COLOR_BUFFER_BIT));

        Renderer renderer;
        
        {   
            glm::mat4 model = glm::translate(glm::mat4(1.0f), m_TranslationA);
            glm::mat4 mvp = m_Proj * m_View * model;
            m_Shader->Bind();
            m_Shader->SetUniformMat4f("u_MVP", mvp);
            renderer.Draw(*m_VAO, *m_IndexBuffer, *m_Shader);
        }

        // {
        //     glm::mat4 model = glm::translate(glm::mat4(1.0f), m_TranslationB);
        //     glm::mat4 mvp = m_Proj * m_View * model;
        //     m_Shader->Bind();
        //     m_Shader->SetUniformMat4f("u_MVP", mvp);
        //     renderer.Draw(*m_VAO, *m_IndexBuffer, *m_Shader);
        // }

    }

    void TestBatchTexture::OnImGuiRender()
    {
        ImGui::SliderFloat3("translationA", &m_TranslationA.x, 0.0f, 960.0f);
        ImGui::SliderFloat3("translationB", &m_TranslationB.x, 0.0f, 960.0f);
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    }
}