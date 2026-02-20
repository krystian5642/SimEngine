#include "Texture.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

Texture::Texture(const std::string& fileLocation, TextureFormat format)
    : textureFormat(format)
{
    LoadFromFile(fileLocation);
}

Texture::Texture(const TextureData& textureData, TextureFormat format)
    : textureData(textureData)
    , textureFormat(format)
{
}

Texture::~Texture()
{
    if (loadedFromFile && textureData.data)
    {
        stbi_image_free(textureData.data);
    }
}

void Texture::LoadFromFile(const std::string& fileLocation)
{
    stbi_set_flip_vertically_on_load(true);
    
    textureData.data = stbi_load(fileLocation.c_str(), &textureData.width
        , &textureData.height, &textureData.bitDepth, 0);
    if (!textureData.data)
    {
        throw std::runtime_error("Failed to load texture " + fileLocation);
    }
    
    loadedFromFile = true;
    
    stbi_set_flip_vertically_on_load(false);
}