#pragma once

#include "Rendering/UniformNames.h"

namespace SimEngine
{
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
        ShadowMap(int width, int height);
        virtual ~ShadowMap() = 0;
    
        virtual void Write() const = 0;
        virtual void Read(unsigned int textureUnit) const = 0;
        
        int GetWidth() const { return width; }
        int GetHeight() const { return height; }
        
    protected:
        int width, height;
    };
}
