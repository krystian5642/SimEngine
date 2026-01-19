#pragma once

#include "Mesh.h"
#include "Shader.h"

class Skybox
{
public:
    Skybox(const std::vector<std::string>& faceLocations);
    virtual ~Skybox() {}
    
    virtual void Draw(const glm::mat4& projectionMatrix, const glm::mat4& viewMatrix) const = 0;

protected:
    virtual void Create(const std::vector<std::string>& faceLocations) = 0;
    
    std::shared_ptr<Mesh> mesh;
    std::shared_ptr<Shader> shader;
};

