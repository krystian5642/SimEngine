#pragma once

#include "Rendering/Core/Texture.h"

class GLTexture : public Texture
{
public:
    GLTexture(const std::string& fileLocation);
    GLTexture(unsigned char* data, int width, int height);
    ~GLTexture() override;
    
    void Bind() const override;

protected:
    void LoadGPUData() override;
    void FreeGPUData() override;
    
private:
    GLuint textureID{};
};
