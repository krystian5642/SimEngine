#include "Texture.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

namespace SimEngine
{
    Texture::Texture(const std::string& fileLocation)
    {
        SetFileLocation(fileLocation, false);
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
        textureData.data = stbi_load(textureData.fileLocation.c_str(), &textureData.width
            , &textureData.height, &textureData.bitDepth, 0);
        if (!textureData.data)
        {
            throw std::runtime_error("Failed to load texture " + textureData.fileLocation);
        }
    }

    void Texture::FreeCPUData()
    {
        stbi_image_free(textureData.data);
    }
}
