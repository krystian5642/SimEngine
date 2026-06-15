#include "Renderer.h"

#include "Scene/Scene.h"
#include "Components/RenderComponent.h"
#include "Core/App.h"
#include "Core/Window.h"

#include <GLFW/glfw3.h>

#include "MeshShader.h"
#include "Shader.h"

void GLScreenRenderData::Reset()
{
    if (quadVAO != 0)
    {
        glDeleteVertexArrays(1, &quadVAO);
        quadVAO = 0;
    }

    if (quadVBO != 0)
    {
        glDeleteBuffers(1, &quadVBO);
        quadVBO = 0;   
    }
    
    if (postProcessingFBO != 0)
    {
        glDeleteFramebuffers(1, &postProcessingFBO);
        postProcessingFBO = 0;
    }
    
    if (postProcessingRBO != 0)
    {
        glDeleteRenderbuffers(1, &postProcessingRBO);
        postProcessingRBO = 0;
    }
    
    if (postProcessingTexture != 0)
    {
        glDeleteTextures(1, &postProcessingTexture);
        postProcessingTexture = 0;
    }
}

void GLAntialiasingData::Reset()
{
    if (FBO != 0)
    {
        glDeleteFramebuffers(1, &FBO);
        FBO = 0;
    }

    if (colorbufferTexture != 0)
    {
        glDeleteTextures(1, &colorbufferTexture);
        colorbufferTexture = 0;
    }

    if (RBO != 0)
    {
        glDeleteRenderbuffers(1, &RBO);
        RBO = 0;
    }
}

Renderer::Renderer()
{

}

Renderer::~Renderer()
{
    ResetRenderBuffer();
}

void Renderer::Init()
{
    auto& window = App::Get().window;
    
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK)
    {
        glfwDestroyWindow(window.GetGLFWWindow());
        glfwTerminate();
        throw std::runtime_error("Failed to initialize GLEW");
    }
    
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    
    InitSceneShaders();
    
    window.onWindowSizeChangedEvent.BindRaw(this, &Renderer::OnWindowSizeChanged);
    InitRenderBuffer(window.GetBufferWidth(), window.GetBufferHeight());
}

void Renderer::SetLineWidth(float width)
{
    glLineWidth(width);   
}

float Renderer::GetLineWidth() const
{
    GLfloat width;
    glGetFloatv(GL_LINE_WIDTH, &width);
    return width;
}

void Renderer::SetPointSize(float size)
{
    glPointSize(size);  
}

float Renderer::GetPointSize() const
{
    GLfloat size;
    glGetFloatv(GL_POINT_SIZE, &size);
    return size;
}

void Renderer::InitRenderBuffer(int bufferWidth, int bufferHeight)
{
    ResetRenderBuffer();
    
    if (bufferWidth <= 0 || bufferHeight <= 0)
    {
        return;    
    }
    
    glGenFramebuffers(1, &antialiasingData.FBO);
    glBindFramebuffer(GL_FRAMEBUFFER, antialiasingData.FBO);

    GLint maxSamples;
    glGetIntegerv(GL_MAX_SAMPLES, &maxSamples);
    
    glGenTextures(1, &antialiasingData.colorbufferTexture);
    glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, antialiasingData.colorbufferTexture);
    glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, static_cast<GLsizei>(maxSamples)
        , GL_RGBA16F, bufferWidth, bufferHeight, GL_TRUE);
    glTexParameteri(GL_TEXTURE_2D_MULTISAMPLE, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D_MULTISAMPLE, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D_MULTISAMPLE, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D_MULTISAMPLE, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, antialiasingData.colorbufferTexture, 0);
    
    constexpr GLuint attachments[2] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
    glDrawBuffers(2, attachments);
    
    glGenRenderbuffers(1, &antialiasingData.RBO);
    glBindRenderbuffer(GL_RENDERBUFFER, antialiasingData.RBO);
    glRenderbufferStorageMultisample(GL_RENDERBUFFER, static_cast<GLsizei>(maxSamples), GL_DEPTH24_STENCIL8, bufferWidth, bufferHeight);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, antialiasingData.RBO);
    
    auto status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if (status != GL_FRAMEBUFFER_COMPLETE)
    {       
        throw std::runtime_error("ERROR: Framebuffer is not complete!! Error code " + std::to_string(status));
    }

    glGenFramebuffers(1, &screenRenderData.postProcessingFBO);
    glBindFramebuffer(GL_FRAMEBUFFER, screenRenderData.postProcessingFBO);

    glGenTextures(1, &screenRenderData.postProcessingTexture);
    glBindTexture(GL_TEXTURE_2D, screenRenderData.postProcessingTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, bufferWidth, bufferHeight, 0, GL_RGBA, GL_FLOAT, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, screenRenderData.postProcessingTexture, 0);

    status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if (status != GL_FRAMEBUFFER_COMPLETE)
    {       
        throw std::runtime_error("ERROR: Framebuffer is not complete!! Error code " + std::to_string(status));
    }
    
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    
    // screen quad
    float quadVertices[] = {  
        // positions   // texCoords
        -1.0f,  1.0f,  0.0f, 1.0f,
        -1.0f, -1.0f,  0.0f, 0.0f,
         1.0f, -1.0f,  1.0f, 0.0f,

        -1.0f,  1.0f,  0.0f, 1.0f,
         1.0f, -1.0f,  1.0f, 0.0f,
         1.0f,  1.0f,  1.0f, 1.0f
    };
    
    glGenVertexArrays(1, &screenRenderData.quadVAO);
    glBindVertexArray(screenRenderData.quadVAO);
    
    glGenBuffers(1, &screenRenderData.quadVBO);
    glBindBuffer(GL_ARRAY_BUFFER, screenRenderData.quadVBO);
    glBufferData(GL_ARRAY_BUFFER, 24 * sizeof(float), quadVertices, GL_STATIC_DRAW);
    
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), nullptr);
    glEnableVertexAttribArray(0);
    
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);
    
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Renderer::RenderScene(const Scene* scene) const
{
    glEnable(GL_DEPTH_TEST);
    
    glBindFramebuffer(GL_FRAMEBUFFER, antialiasingData.FBO);
 
    glClearColor(clearColor.x, clearColor.y, clearColor.z, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    const auto& window = App::Get().window;
    glViewport(0, 0, window.GetBufferWidth(), window.GetBufferHeight());

    Render(scene);
    
    const auto bufferWidth = window.GetBufferWidth();
    const auto bufferHeight = window.GetBufferHeight();
    
    glBindFramebuffer(GL_READ_FRAMEBUFFER, antialiasingData.FBO);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, screenRenderData.postProcessingFBO);
        
    glReadBuffer(GL_COLOR_ATTACHMENT0);
    glDrawBuffer(GL_COLOR_ATTACHMENT0);
    glBlitFramebuffer(0, 0, bufferWidth, bufferHeight
        , 0, 0, bufferWidth, bufferHeight, GL_COLOR_BUFFER_BIT, GL_NEAREST);
        
    glReadBuffer(GL_COLOR_ATTACHMENT1);
    glDrawBuffer(GL_COLOR_ATTACHMENT1);
    glBlitFramebuffer(0, 0, bufferWidth, bufferHeight
        , 0, 0, bufferWidth, bufferHeight, GL_COLOR_BUFFER_BIT, GL_NEAREST);
    
    glBindVertexArray(screenRenderData.quadVAO);
    glDisable(GL_DEPTH_TEST);
    glActiveTexture(GL_TEXTURE0);
    
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    
    auto shader = sceneShaders.screenShader;
    
    shader->BindAndValidate();
    
    glBindVertexArray(screenRenderData.quadVAO);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glDisable(GL_DEPTH_TEST);
    glClear(GL_COLOR_BUFFER_BIT);
    
    glDrawArrays(GL_TRIANGLES, 0, 6);
    
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, screenRenderData.postProcessingTexture);
}

void Renderer::InitSceneShaders()
{
    ShaderData meshShaderData;
    meshShaderData.vertShader = ShaderData::shadersFolder + "mesh.vert";
    
    meshShaderData.fragShader = ShaderData::shadersFolder + "mesh.frag";
    sceneShaders.meshShader = std::make_shared<MeshShader>(meshShaderData);
    
    ShaderData screenShaderData;
    screenShaderData.vertShader = ShaderData::shadersFolder + "screen.vert";
    screenShaderData.fragShader = ShaderData::shadersFolder + "screen.frag";
    sceneShaders.screenShader = std::make_shared<Shader>(screenShaderData);   
}

void Renderer::Render(const Scene* scene) const
{
    const auto& renderData = scene->GetRenderData();
    for (auto* renderComponent : renderData.renderComponents)
    {
        renderComponent->Draw();
    }
}

void Renderer::ResetRenderBuffer()
{
    screenRenderData.Reset();
    antialiasingData.Reset();
}

void Renderer::OnWindowSizeChanged(Window* window, int bufferWidth, int bufferHeight)
{
    InitRenderBuffer(bufferWidth, bufferHeight);
}