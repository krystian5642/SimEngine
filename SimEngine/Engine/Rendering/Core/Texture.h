#pragma once
#include "Rendering/Renderer/Renderer.h"

struct TextureData
{
    unsigned char* data{};
    int width{}, height{}, bitDepth{};
};

class Texture
{
public:
    Texture(const std::string& fileLocation);
    Texture(const TextureData& textureData);
    virtual ~Texture();
    
    virtual void Bind() const = 0;
    
    static TexturePtr CreateTexture(const std::string& fileLocation)
    {
        return Renderer::Get()->CreateTexture(fileLocation);
    }
    
    static TexturePtr CreateTexture(const TextureData& textureData)
    {
        return Renderer::Get()->CreateTexture(textureData);
    }

protected:
    void LoadFromFile(const std::string& fileLocation);
    
    TextureData textureData;
    
private:
    bool loadedFromFile{false};
};
