#pragma once

class Skybox
{
public:
    Skybox(const std::vector<std::string>& faceLocations);
    virtual void Draw(const glm::mat4& projectionMatrix, const glm::mat4& viewMatrix) const = 0;

protected:
    virtual void Create(const std::vector<std::string>& faceLocations) = 0;
    
    MeshPtr mesh;
    ShaderPtr shader;
};

