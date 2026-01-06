#pragma once

#include "Rendering/Core/Skybox.h"

namespace SimEngine
{
    class GLSkybox : public Skybox
    {
    public:
        GLSkybox(const std::vector<std::string>& faceLocations);
        ~GLSkybox() override;
        
        void Draw(const glm::mat4& projectionMatrix, const glm::mat4& viewMatrix) const override;
        
        GLuint GetTextureId() const { return textureId; }
    
    private:
        void Create(const std::vector<std::string>& faceLocations) override;
        
        GLuint textureId{};
    };
}
