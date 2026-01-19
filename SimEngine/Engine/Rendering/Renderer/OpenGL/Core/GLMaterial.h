#pragma once

#include "Rendering/Core/Material.h"

class GLReflectMaterial : public Material
{
public:
    GLReflectMaterial(const MaterialResources& resources);
    
    void Use(const std::shared_ptr<const Shader>& shader) const override;
};

class GLRefractMaterial : public Material
{
public:
    GLRefractMaterial(const MaterialResources& resources);
    
    void Use(const std::shared_ptr<const Shader>& shader) const override;
};

