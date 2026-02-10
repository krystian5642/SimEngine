#pragma once

#include "Rendering/UniformNames.h"

class Texture;

namespace UniformNames
{
    UNIFORM_NAME materialAmbient = "material.ambient";
    UNIFORM_NAME materialDiffuse = "material.diffuse";
    UNIFORM_NAME materialSpecular = "material.specular";
    UNIFORM_NAME materialShininess = "material.shininess";
    UNIFORM_NAME materialTexture = "material.texture";
    UNIFORM_NAME materialUseTexture = "material.useTexture";
}

struct MaterialData
{
    glm::vec3 ambient{1.0f};
    glm::vec3 diffuse{1.0f};
    glm::vec3 specular{1.0f};
    float shininess{1.0f};
};

struct MaterialResources
{
    TexturePtr texture;
    std::shared_ptr<const Shader> shader;
    MaterialData data;
};

class Material
{
public:
    Material(const MaterialResources& resources) : resources(resources) {}
    virtual ~Material() {}
    
    virtual void Use(const std::shared_ptr<const Shader>& shader) const;
    
    const MaterialResources& GetResources() const { return resources; }
    
protected:
    MaterialResources resources;
    
};

class ReflectMaterial : public Material
{
public:
    ReflectMaterial(const MaterialResources& resources) : Material(resources) {}
    virtual ~ReflectMaterial() = 0 {}
    
    static MaterialPtr CreateReflectMaterial();
};

class RefractMaterial : public Material
{
public:
    RefractMaterial(const MaterialResources& resources) : Material(resources) {}
    virtual ~RefractMaterial() = 0 {}
    
    static MaterialPtr CreateRefractMaterial();
};
