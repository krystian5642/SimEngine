#pragma once
#include "Rendering/UniformNames.h"

class Shader;

namespace UniformNames
{
    UNIFORM_NAME lineColor = "lineColor";
}

class Line
{
public:
    Line();
    ~Line();
    
    virtual void Draw() const;
    
    const std::vector<glm::vec3>& GetPoints() const { return points; }
    void SetPoints(const std::vector<glm::vec3>& newPoints);
    
    void ClearPoints() { points.clear(); }
    
    void AddPoint(const glm::vec3& point);
    
    glm::vec3 color{1.0f};
    float thickness{1.0f};
    float maxLength{-1.0f};
    
protected:
    // Points need to be in the cartesian coordinate system
    std::vector<glm::vec3> points; 
    std::shared_ptr<Shader> shader;
    
    float currentLength{0.0f};
    
private:
    GLuint VAO{};
    GLuint VBO{};
};
