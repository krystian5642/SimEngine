#include "GLShadowMap.h"

namespace SimEngine
{
    GLShadowMap::GLShadowMap(int width, int height)
        : ShadowMap(width, height)
    {
        glGenFramebuffers(1, &shadowMapData.FBO);
    
        glGenTextures(1, &shadowMapData.shadowMap);
        glBindTexture(GL_TEXTURE_2D, shadowMapData.shadowMap);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
    
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
        float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
        glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, shadowMapData.FBO);
        glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, shadowMapData.shadowMap, 0);
    
        glDrawBuffer(GL_NONE);
        glReadBuffer(GL_NONE);

        GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
        if (status != GL_FRAMEBUFFER_COMPLETE)
        {
            std::cout << "ERROR::ShadowMap: Framebuffer not complete!" << " ERROR code " << status << std::endl;
        }
        
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    GLShadowMap::~GLShadowMap()
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

    void GLShadowMap::Write() const
    {
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, shadowMapData.FBO);
    }

    void GLShadowMap::Read(unsigned int textureUnit) const
    {
        glActiveTexture(textureUnit);
        glBindTexture(GL_TEXTURE_2D, shadowMapData.shadowMap);
    }
}
