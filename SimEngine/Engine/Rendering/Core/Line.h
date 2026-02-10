#pragma once
#include "Rendering/UniformNames.h"
#include "Rendering/Renderer/Renderer.h"

class Shader;

namespace UniformNames
{
    UNIFORM_NAME lineColor = "lineColor";
}

class Line
{
public:
    Line() {}
    virtual ~Line() {}
    
    static LinePtr CreateLine()
    {
        return Renderer::Get()->CreateLine();
    }
    
    virtual void Draw() const = 0;
    
    const std::vector<glm::vec3>& GetPoints() const { return points; }
    void SetPoints(const std::vector<glm::vec3>& newPoints);
    
    void ClearPoints() { points.clear(); }
    
    void AddPoint(const glm::vec3& point);
    
    glm::vec3 color{1.0f};
    float thickness{1.0f};
    float maxLength{-1.0f};
    
protected:
    std::vector<glm::vec3> points;
    ShaderPtr shader;
    
    float currentLength{0.0f};
};
