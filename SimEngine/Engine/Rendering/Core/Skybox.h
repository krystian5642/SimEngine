#pragma once
#include "Rendering/Renderer/Renderer.h"

class Skybox
{
public:
    Skybox(const std::vector<std::string>& faceLocations) {}
    virtual ~Skybox() {}
    
    static SkyboxPtr CreateSkybox(const std::vector<std::string>& faceLocations)
    {
        return Renderer::Get()->CreateSkybox(faceLocations);
    }
    
    virtual void Draw(const glm::mat4& projectionMatrix, const glm::mat4& viewMatrix) const = 0;

protected:
    virtual void Create(const std::vector<std::string>& faceLocations) = 0;
    
    MeshPtr mesh;
    ShaderPtr shader;
};

