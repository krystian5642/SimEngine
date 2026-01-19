#pragma once

#include "AssetManager.h"

class Material;

class MaterialManager : public AssetManager<Material>
{
public:
    static MaterialManager& Get();

private:
    MaterialManager();
};
