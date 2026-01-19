#include "GLTexture.h"

GLTexture::GLTexture(const std::string& fileLocation)
    : Texture(fileLocation)
{
}

GLTexture::~GLTexture()
{
    GLTexture::FreeGPUData();
}

void GLTexture::Bind() const
{
    if (textureID == 0)
    {
        throw std::runtime_error("Can't use texture, texture not loaded");
    }

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureID);
}

void GLTexture::LoadGPUData()
{
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, textureData.width
        , textureData.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, textureData.data);
    glGenerateMipmap(GL_TEXTURE_2D);

    glBindTexture(GL_TEXTURE_2D, 0);
}

void GLTexture::FreeGPUData()
{
    glDeleteTextures(1, &textureID);
}