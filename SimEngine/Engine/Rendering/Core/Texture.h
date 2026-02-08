#pragma once

struct TextureData
{
    std::string fileLocation;
    unsigned char* data{};
    int width, height, bitDepth;
};

class Texture
{
public:
    Texture(const std::string& fileLocation);
    Texture(unsigned char* data, int width, int height);
    virtual ~Texture();

    virtual void Bind() const = 0;
    
    void Load();

    void SetFileLocation(const std::string& newLocation, bool reload = true);

protected:
    virtual void LoadGPUData() = 0;
    virtual void FreeGPUData() = 0;
    
    void LoadCPUData();
    void FreeCPUData();
    
    TextureData textureData;
    
    bool stbiIMAGELoaded{false};
};
