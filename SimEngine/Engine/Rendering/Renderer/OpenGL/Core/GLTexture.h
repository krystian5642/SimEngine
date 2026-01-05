#pragma once

#include "Rendering/Core/Texture.h"

namespace SimEngine
{
    class GLTexture : public Texture
    {
    public:
        GLTexture(const std::string& fileLocation);
        ~GLTexture() override;
        
        void Bind() const override;

    protected:
        void LoadGPUData() override;
        void FreeGPUData() override;
        
    private:
        GLuint textureID{};
    };
}
