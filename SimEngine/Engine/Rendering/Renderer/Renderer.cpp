#include "Renderer.h"

#include "OpenGL/OpenGLRenderer.h"

void Renderer::Init(RendererType type)
{
    switch (type)
    {
        case RendererType::OpenGL:
            rendererAPI = std::make_unique<OpenGLRenderer>();
            break;  
        default: break;
    }
}
