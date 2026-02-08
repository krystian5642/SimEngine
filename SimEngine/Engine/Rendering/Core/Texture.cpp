#include "Texture.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

Texture::Texture(const std::string& fileLocation)
{
    SetFileLocation(fileLocation, false);
}

Texture::Texture(unsigned char* data, int width, int height)
{
    textureData.data = data;
    textureData.width = width;
    textureData.height = height;
}

Texture::~Texture()
{
    FreeCPUData();
}

void Texture::Load()
{
    FreeCPUData();
    FreeGPUData();
    
    LoadCPUData();
    LoadGPUData();
}

void Texture::SetFileLocation(const std::string& newLocation, bool reload)
{
    textureData.fileLocation = newLocation;
    if (reload)
    {
        Load();
    }
}

void Texture::LoadCPUData()
{
    stbi_set_flip_vertically_on_load(true);
    
    textureData.data = stbi_load(textureData.fileLocation.c_str(), &textureData.width
        , &textureData.height, &textureData.bitDepth, 0);
    if (!textureData.data)
    {
        throw std::runtime_error("Failed to load texture " + textureData.fileLocation);
    }
    
    stbi_set_flip_vertically_on_load(false);
    
    stbiIMAGELoaded = true;
}

void Texture::FreeCPUData()
{
    if (stbiIMAGELoaded)
    {
        stbi_image_free(textureData.data);
    }
}