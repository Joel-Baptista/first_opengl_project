#pragma once

#include "Test.h"
#include "../VertexBufferLayout.h"
#include "../VertexBuffer.h"
#include "../Texture.h"

#include <memory>

#include "../include/glm/glm.hpp"
#include "../include/glm/gtc/matrix_transform.hpp"


namespace test {

    class TestBatchRendering2D : public Test{
        public:
            TestBatchRendering2D();
            ~TestBatchRendering2D();

            void OnUpdate(float deltaTime) override;
            void OnRender() override;
            void OnImGuiRender() override;

        private:
            std::unique_ptr<VertexArray> m_VAO;
            std::unique_ptr<IndexBuffer> m_IndexBuffer;
            std::unique_ptr<VertexBuffer> m_VertexBuffer;
            std::unique_ptr<Shader> m_Shader;
            std::unique_ptr<Texture> m_Texture;
            
            glm::mat4 m_Proj, m_View;
            glm::vec3 m_TranslationA, m_TranslationB;
    };
}