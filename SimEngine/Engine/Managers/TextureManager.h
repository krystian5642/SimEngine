#pragma once

#include "AssetManager.h"

class Texture;

class TextureManager : public AssetManager<Texture>
{
public:
    static TextureManager& Get();

private:
    TextureManager() {}
};
