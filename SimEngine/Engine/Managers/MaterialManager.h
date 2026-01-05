#pragma once

#include "AssetManager.h"

namespace SimEngine
{
    class Material;

    class MaterialManager : public AssetManager<Material>
    {
    public:
        static MaterialManager& Get();
    
    private:
        MaterialManager();
    };
}
