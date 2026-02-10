#pragma once

#include "Rendering/Core/Texture.h"

class GLTexture : public Texture
{
public:
    GLTexture(const std::string& fileLocation);
    GLTexture(const TextureData& textureData);
    ~GLTexture() override;
    
    void Bind() const override;
    
private:
    void GenerateTexture();
    
    GLuint textureID{};
};
