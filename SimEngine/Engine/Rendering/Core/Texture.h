#pragma once
#include "Rendering/Renderer/Renderer.h"

struct TextureData
{
    unsigned char* data{};
    int width{}, height{}, bitDepth{};
};

enum class TextureFormat
{
    RGB, 
    RGBA,
    sRGB
};

class Texture
{
public:
    Texture(const std::string& fileLocation, TextureFormat format = TextureFormat::RGB);
    Texture(const TextureData& textureData, TextureFormat format = TextureFormat::RGB);
    virtual ~Texture();
    
    virtual void Bind(unsigned int offset = 0) const = 0;
    
    static TexturePtr CreateTexture(const std::string& fileLocation, TextureFormat format = TextureFormat::RGB)
    {
        return Renderer::Get()->CreateTexture(fileLocation, format);
    }
    
    static TexturePtr CreateTexture(const TextureData& textureData, TextureFormat format = TextureFormat::RGB)
    {
        return Renderer::Get()->CreateTexture(textureData, format);
    }

protected:
    void LoadFromFile(const std::string& fileLocation);
    
    TextureData textureData;
    const TextureFormat textureFormat;
    
    bool loadedFromFile{false};
};
