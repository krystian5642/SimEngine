#pragma once

#include "Rendering/Core/Texture.h"

class GLTexture : public Texture
{
public:
    GLTexture(const std::string& fileLocation, TextureFormat format);
    GLTexture(const TextureData& textureData, TextureFormat format);
    ~GLTexture() override;
    
    void Bind(unsigned int offset = 0) const override;
    
private:
    void GenerateTexture();
    GLint GetGLTextureFormat() const;
    
    GLuint textureID{};
};
