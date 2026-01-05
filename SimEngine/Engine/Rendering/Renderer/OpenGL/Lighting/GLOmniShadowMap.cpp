#include "GLOmniShadowMap.h"

#include <iostream>
#include <ostream>

namespace SimEngine
{
    GLOmniShadowMap::GLOmniShadowMap(int width, int height)
        : ShadowMap(width, height)
    {
        glGenFramebuffers(1, &shadowMapData.FBO);

        glGenTextures(1, &shadowMapData.shadowMap);
        glBindTexture(GL_TEXTURE_CUBE_MAP, shadowMapData.shadowMap);

        for (size_t i = 0; i < 6; i++)
        {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT
                , width, width, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
        }

        glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    
        glBindFramebuffer(GL_FRAMEBUFFER, shadowMapData.FBO);
        glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, shadowMapData.shadowMap, 0);
    
        glDrawBuffer(GL_NONE);
        glReadBuffer(GL_NONE);

        GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
        if (status != GL_FRAMEBUFFER_COMPLETE)
        {
            std::cout << "ERROR::ShadowMap: Framebuffer not complete!" << " ERROR code " << status << std::endl;
        }
    }

    GLOmniShadowMap::~GLOmniShadowMap()
    {
        if (shadowMapData.FBO != 0)
        {
            glDeleteFramebuffers(1, &shadowMapData.FBO);
        }

        if (shadowMapData.shadowMap != 0)
        {
            glDeleteTextures(1, &shadowMapData.shadowMap);
        }
    }

    void GLOmniShadowMap::Write() const
    {
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, shadowMapData.FBO);
    }

    void GLOmniShadowMap::Read(unsigned int textureUnit) const
    {
        glActiveTexture(textureUnit);
        glBindTexture(GL_TEXTURE_CUBE_MAP, shadowMapData.shadowMap);
    }
}
