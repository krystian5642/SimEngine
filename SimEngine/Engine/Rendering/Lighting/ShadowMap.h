#pragma once

#include "Rendering/UniformNames.h"
#include "Rendering/Renderer/Renderer.h"

namespace UniformNames
{
    UNIFORM_NAME directionalLightProjection = "directionalLightProjection";
    UNIFORM_NAME directionalShadowMap = "directionalShadowMap";
    
    UNIFORM_NAME omniShadowMap = "omniShadowMap.shadowMap";
    UNIFORM_NAME lightPos = "lightPos";
}

class ShadowMap
{
public:
    ShadowMap(int width, int height) 
        : width(width)
        , height(height)
    {}
    
    virtual ~ShadowMap() {}
    
    static ShadowMapPtr CreateShadowMap(int width, int height)
    {
        return Renderer::Get()->CreateShadowMap(width, height);
    }
    
    static ShadowMapPtr CreateOmniShadowMap(int width, int height)
    {
        return Renderer::Get()->CreateOmniShadowMap(width, height);
    }
    
    virtual void Write() const = 0;
    virtual void Read(unsigned int textureUnit) const = 0;
    
    int GetWidth() const { return width; }
    int GetHeight() const { return height; }
    
protected:
    int width, height;
 
};
