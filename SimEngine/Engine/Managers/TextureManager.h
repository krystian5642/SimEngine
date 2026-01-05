#pragma once

#include "AssetManager.h"

namespace SimEngine
{
    class Texture;

    class TextureManager : public AssetManager<Texture>
    {
    public:
        static TextureManager& Get();
    
    private:
        TextureManager();
    };
}
