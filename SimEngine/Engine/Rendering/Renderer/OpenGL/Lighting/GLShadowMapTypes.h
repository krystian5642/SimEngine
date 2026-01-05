#pragma once
#include <GL/glew.h>

namespace SimEngine
{
    struct GLShadowMapData
    {
        GLuint FBO{};
        GLuint shadowMap{};
    };
}
