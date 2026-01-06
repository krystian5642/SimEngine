#pragma once

#include "Rendering/Core/Material.h"

namespace SimEngine
{
    class GLReflectMaterial : public Material
    {
    public:
        GLReflectMaterial(const MaterialResources& resources);
        ~GLReflectMaterial() override;
        
        void Use(const std::shared_ptr<const Shader>& shader) const override;
    };
    
    class GLRefractMaterial : public Material
    {
    public:
        GLRefractMaterial(const MaterialResources& resources);
        ~GLRefractMaterial() override;
        
        void Use(const std::shared_ptr<const Shader>& shader) const override;
    };
}

