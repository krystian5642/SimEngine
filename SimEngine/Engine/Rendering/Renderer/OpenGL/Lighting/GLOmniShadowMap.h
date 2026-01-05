#pragma once

#include "GLShadowMapTypes.h"
#include "Rendering/Lighting/ShadowMap.h"

namespace SimEngine
{
    class GLOmniShadowMap : public ShadowMap
    {
    public:
        GLOmniShadowMap(int width, int height);
        ~GLOmniShadowMap() override;
        
        void Write() const override;
        void Read(unsigned int textureUnit) const override;
        
    private:
        GLShadowMapData shadowMapData;
    };
}
