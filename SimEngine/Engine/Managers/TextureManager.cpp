#include "TextureManager.h"

TextureManager& TextureManager::Get()
{
    static TextureManager textureManager;
    return textureManager;
}
