#pragma once

#include "Rendering/UniformNames.h"

class Texture;

using ConstShaderPtr = std::shared_ptr<const Shader>;

namespace UniformNames
{
    UNIFORM_NAME materialAmbient = "material.ambient";
    UNIFORM_NAME materialDiffuse = "material.diffuse";
    UNIFORM_NAME materialSpecular = "material.specular";
    UNIFORM_NAME materialShininess = "material.shininess";
    UNIFORM_NAME materialDiffuseTexture = "material.diffuseTexture";
    UNIFORM_NAME materialNormalTexture = "material.normalTexture";
    UNIFORM_NAME materialUseDiffuseTexture = "material.useDiffuseTexture";
    UNIFORM_NAME materialUseNormalTexture = "material.useNormalTexture";
    UNIFORM_NAME materialUseParallaxMapping = "material.useParallaxMapping";
    UNIFORM_NAME materialDisplacementTexture = "material.displacementTexture";
    
    UNIFORM_NAME parallaxHeightScale = "material.parallaxMapping.heightScale";
    UNIFORM_NAME parallaxMinLayers = "material.parallaxMapping.minLayers";
    UNIFORM_NAME parallaxMaxLayers = "material.parallaxMapping.maxLayers";
    UNIFORM_NAME parallaxEnabled = "material.parallaxMapping.enabled";
    UNIFORM_NAME parallaxDiscardFragments = "material.parallaxMapping.discardFragments";
}

struct MaterialData
{
    glm::vec3 ambient{1.0f};
    glm::vec3 diffuse{1.0f};
    glm::vec3 specular{1.0f};
    float shininess{1.0f};
};

struct ParallaxMappingData
{
    float heightScale{0.2f};
    int minLayers{32};
    int maxLayers{64};
    bool enabled{true};
    bool discardFragments{true};
};

struct MaterialResources
{
    TexturePtr diffuseTexture;
    TexturePtr normalTexture;
    TexturePtr displacementTexture;
    ConstShaderPtr shader;
    MaterialData data;
    ParallaxMappingData parallaxMappingData;
    bool canUseNormalMap{true};
};

class Material
{
public:
    Material(const MaterialResources& resources) : resources(resources) {}
    virtual ~Material() {}
    
    virtual void Use(const ConstShaderPtr& shader) const;
    
    MaterialResources& GetResources() { return resources; }
    
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
