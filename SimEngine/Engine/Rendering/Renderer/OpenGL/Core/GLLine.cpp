#include "GLLine.h"

#include "GLShader.h"
#include "Rendering/UniformNames.h"
#include "Rendering/Core/Shader.h"
#include "Scene/SceneManager.h"
#include "Scene/Scene.h"
#include "Components/CameraComponent.h"
#include "Core/App.h"

namespace SimEngine
{
    GLLine::GLLine()
    {
        const std::string shadersFolder = "Engine/Rendering/Shaders/";
        
        ShaderData shaderData;
        shaderData.vertShader = shadersFolder + "line.vert";
        shaderData.fragShader = shadersFolder + "line.frag";
        shader = std::make_shared<GLShader>(shaderData);
    
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);

        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, 0, nullptr, GL_DYNAMIC_DRAW);

        glVertexAttribPointer(0, 3,GL_FLOAT, GL_FALSE, sizeof(glm::vec3), nullptr);
    
        glEnableVertexAttribArray(0);
        glBindVertexArray(0);
    }

    GLLine::~GLLine()
    {
        if (VAO != 0)
        {
            glDeleteVertexArrays(1, &VAO);
        }
    
        if (VBO != 0)
        {
            glDeleteBuffers(1, &VBO);
        }
    }

    void GLLine::Draw() const
    {
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizei>(points.size() * sizeof(glm::vec3)), points.data(), GL_DYNAMIC_DRAW);
        
        shader->Bind();
    
        const glm::mat4 projection = glm::perspective(glm::radians(45.0f), App::GetCurrentWindow()->GetAspectRatio(), 0.1f, 10000.0f);
        const auto& view = SceneManager::GetCurrentScene()->GetActiveCamera()->GetView();
    
        shader->SetMat4f(UniformNames::projection, projection);
        shader->SetMat4f(UniformNames::view, view);
        shader->SetVec3f(UniformNames::lineColor, color);
        
        glLineWidth(thickness);
        glBindVertexArray(VAO);
        glDrawArrays(GL_LINE_STRIP, 0, static_cast<GLsizei>(points.size()));
    
        shader->Unbind();
        
        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }
}
