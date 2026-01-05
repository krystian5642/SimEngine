#include "TextureManager.h"

namespace SimEngine
{
    TextureManager& TextureManager::Get()
    {
        static TextureManager textureManager;
        return textureManager;
    }

    TextureManager::TextureManager()
    {
    }
}
