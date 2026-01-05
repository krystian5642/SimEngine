#pragma once

#include "GLShadowMapTypes.h"
#include "Rendering/Lighting/ShadowMap.h"

namespace SimEngine
{
    class GLShadowMap : public ShadowMap
    {
    public:
        GLShadowMap(int width, int height);
        ~GLShadowMap() override;
        
        void Write() const override;
        void Read(unsigned int textureUnit) const override;
        
    private:
        GLShadowMapData shadowMapData;
    };
}
