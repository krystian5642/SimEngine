#include "GLTexture.h"

#include <stb_image.h>

GLTexture::GLTexture(const std::string& fileLocation)
    : Texture(fileLocation)
{
    GenerateTexture();
}

GLTexture::GLTexture(const TextureData& textureData)
    : Texture(textureData)
{
    GenerateTexture();
}

GLTexture::~GLTexture()
{
    glDeleteTextures(1, &textureID);
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

void GLTexture::GenerateTexture()
{
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, textureData.width
        , textureData.height, 0, GL_RGB, GL_UNSIGNED_BYTE, textureData.data);
    
    glGenerateMipmap(GL_TEXTURE_2D);
    
    GLint maxAnisotropy;
    glGetIntegerv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &maxAnisotropy);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, maxAnisotropy);

    glBindTexture(GL_TEXTURE_2D, 0);
    
    if (loadedFromFile)
    {
        stbi_image_free(textureData.data);
        textureData.data = nullptr;
    }
}
