#include "Line.h"

#include "Shader.h"
#include "Scene/SceneManager.h"
#include "Scene/Scene.h"

Line::Line()
{
    ShaderData shaderData;
    shaderData.vertShader = ShaderData::shadersFolder + "line.vert";
    shaderData.fragShader = ShaderData::shadersFolder + "line.frag";
    shader = std::make_shared<Shader>(shaderData);

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, 0, nullptr, GL_DYNAMIC_DRAW);

    glVertexAttribPointer(0, 3,GL_FLOAT, GL_FALSE, sizeof(glm::vec3), nullptr);

    glEnableVertexAttribArray(0);
    glBindVertexArray(0);
}

Line::~Line()
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

void Line::Draw() const
{
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizei>(points.size() * sizeof(glm::vec3)), points.data(), GL_DYNAMIC_DRAW);
    
    shader->BindAndValidate();

    const auto currentScene = SceneManager::GetCurrentScene();
    
    const auto projection = currentScene->GetProjectionMatrix();
    const auto& view = currentScene->GetViewMatrix();

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

void Line::SetPoints(const std::vector<glm::vec3>& newPoints)
{
    if (maxLength > 0.0f && newPoints.size() > 1)
    {
        points.clear();
        points.reserve(newPoints.size());
        
        points.push_back(newPoints[0]);
    
        currentLength = 0.0f;
        for (size_t i = 1; i < newPoints.size(); i++)
        {
            currentLength += glm::distance(newPoints[i - 1], newPoints[i]);
            if (currentLength < maxLength)
            {
                points.push_back(newPoints[i]);
            }
        }
    }
    else
    {
        currentLength = 0.0f;
        points = newPoints;
        
        if (!points.empty())
        {
            for (size_t i = 1; i < newPoints.size(); i++)
            {
                currentLength += glm::distance(newPoints[i - 1], newPoints[i]);
            }
        }
    }
}

void Line::AddPoint(const glm::vec3& point)
{
    if (maxLength > 0.0f)
    {
        if (!points.empty())
        {
            const auto& last = points.back();
            const float lastSegmentLength = glm::distance(last, point);
            while (points.size() > 1 && currentLength + lastSegmentLength > maxLength)
            {
                const auto& first = points[0];
                const auto& second = points[1];
        
                const float firstSegmentLength = glm::distance(first, second);
                currentLength -= firstSegmentLength;
        
                points.erase(points.begin());
            }
        
            currentLength += lastSegmentLength;
            points.push_back(point);
        }
        else
        {
            currentLength = 0.0f;
            points.push_back(point);
        }
    }
    else
    {
        if (!points.empty())
        {
            currentLength += glm::distance(points.back(), point);
        }
        points.push_back(point);
    }
}