#include "OpenGLRenderer.h"

#include "Rendering/Renderer/OpenGL/Core/GLMesh.h"
#include "Rendering/Renderer/OpenGL/Core/GLTexture.h"
#include "Rendering/Renderer/OpenGL/Core/GLShader.h"
#include "Rendering/Renderer/OpenGL/Core/GLSkybox.h"
#include "Rendering/Renderer/OpenGL/Lighting/GLOmniShadowMap.h"
#include "Rendering/Renderer/OpenGL/Lighting/GLShadowMap.h"
#include "Scene/Scene.h"
#include "Scene/Objects/Lighting/DirectionalLightObject.h"
#include "Scene/Objects/Lighting/PointLightObject.h"
#include "Components/RenderComponent.h"
#include "Components/CameraComponent.h"
#include "Core/App.h"
#include "Core/Window.h"

#include <GLFW/glfw3.h>

#include "Core/GLInstancedMesh.h"
#include "Core/GLLine.h"
#include "Core/GLMaterial.h"

#define CREATE_SHADER(shaderData) std::dynamic_pointer_cast<const GLShader>(CreateShader(shaderData))

OpenGLRenderer::OpenGLRenderer()
{
    auto window = App::GetCurrentWindow();
    
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK)
    {
        glfwDestroyWindow(window->GetGLFWWindow());
        glfwTerminate();
        throw std::runtime_error("Failed to initialize GLEW");
    }
    
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    
    OpenGLRenderer::InitSceneShaders();
    
    window->onWindowSizeChangedEvent.BindRaw(this, &OpenGLRenderer::OnWindowSizeChanged);
    InitRenderBuffer(window->GetBufferWidth(), window->GetBufferHeight());
}

OpenGLRenderer::~OpenGLRenderer()
{
    ResetRenderBuffer();
}

void OpenGLRenderer::SetAntiAliasingEnabled(bool enabled)
{
    if (enabled != antialiasingData.enabled)
    {
        antialiasingData.enabled = enabled;
    
        const auto window = App::GetCurrentWindow();
        InitRenderBuffer(window->GetBufferWidth(), window->GetBufferHeight());
    }
}

bool OpenGLRenderer::GetAntiAliasingEnabled() const
{
    return antialiasingData.enabled;
}

TexturePtr OpenGLRenderer::CreateTexture(const std::string& fileLocation) const
{
    return std::make_shared<GLTexture>(fileLocation);
}

TexturePtr OpenGLRenderer::CreateTexture(const TextureData& textureData) const
{
    return std::make_shared<GLTexture>(textureData);
}

ShaderPtr OpenGLRenderer::CreateShader(const ShaderData& shaderData) const
{
    return std::make_shared<GLShader>(shaderData);
}
        
MeshPtr OpenGLRenderer::CreateMesh(const MeshData& meshData) const
{
    return std::make_shared<GLMesh>(meshData);
}

InstancedMeshPtr OpenGLRenderer::CreateInstancedMesh(MeshPtr mesh, const std::vector<Transform>& transforms) const
{
    return std::make_shared<GLInstancedMesh>(mesh, transforms);
}

LinePtr OpenGLRenderer::CreateLine() const
{
    return std::make_unique<GLLine>();
}
    
SkyboxPtr OpenGLRenderer::CreateSkybox(const std::vector<std::string>& faceLocations) const
{
    return std::make_unique<GLSkybox>(faceLocations);
}
    
ShadowMapPtr OpenGLRenderer::CreateShadowMap(int width, int height) const
{
    return std::make_shared<GLShadowMap>(width, height);
}
    
ShadowMapPtr OpenGLRenderer::CreateOmniShadowMap(int width, int height) const
{
    return std::make_shared<GLOmniShadowMap>(width, height);
}
    
MaterialPtr OpenGLRenderer::CreateRefractMaterial(const MaterialResources& resources) const
{
    return std::make_shared<GLRefractMaterial>(resources);
}
    
MaterialPtr OpenGLRenderer::CreateReflectMaterial(const MaterialResources& resources) const
{
    return std::make_shared<GLReflectMaterial>(resources);
}

void OpenGLRenderer::InitRenderBuffer(int bufferWidth, int bufferHeight)
{
    ResetRenderBuffer();
    
    if (antialiasingData.enabled)
    {
        glGenFramebuffers(1, &antialiasingData.FBO);
        glBindFramebuffer(GL_FRAMEBUFFER, antialiasingData.FBO);
    
        glGenTextures(1, &antialiasingData.colorbufferTexture);
        glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, antialiasingData.colorbufferTexture);
        glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, antialiasingData.samples, GL_RGB, bufferWidth, bufferHeight, GL_TRUE);
        glTexParameteri(GL_TEXTURE_2D_MULTISAMPLE, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D_MULTISAMPLE, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D_MULTISAMPLE, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D_MULTISAMPLE, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, antialiasingData.colorbufferTexture, 0);
        
        glGenRenderbuffers(1, &antialiasingData.RBO);
        glBindRenderbuffer(GL_RENDERBUFFER, antialiasingData.RBO);
        glRenderbufferStorageMultisample(GL_RENDERBUFFER, antialiasingData.samples, GL_DEPTH24_STENCIL8, bufferWidth, bufferHeight);
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
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, bufferWidth, bufferHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
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
    }
    else
    {
        glGenFramebuffers(1, &screenRenderData.postProcessingFBO);
        glBindFramebuffer(GL_FRAMEBUFFER, screenRenderData.postProcessingFBO);

        glGenTextures(1, &screenRenderData.postProcessingTexture);
        glBindTexture(GL_TEXTURE_2D, screenRenderData.postProcessingTexture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, bufferWidth, bufferHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glBindTexture(GL_TEXTURE_2D, 0);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, screenRenderData.postProcessingTexture, 0);

        glGenRenderbuffers(1, &screenRenderData.postProcessingRBO);
        glBindRenderbuffer(GL_RENDERBUFFER, screenRenderData.postProcessingRBO);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, bufferWidth, bufferHeight);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, screenRenderData.postProcessingRBO);

        auto status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
        if (status != GL_FRAMEBUFFER_COMPLETE)
        {       
            throw std::runtime_error("ERROR: Framebuffer is not complete!! Error code " + std::to_string(status));
        }
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

void OpenGLRenderer::RenderScene(const Scene* scene) const
{
    glEnable(GL_DEPTH_TEST);
    
    const auto& lightData = scene->GetLightsData();
    
    for (auto* dirLight : lightData.dirLights)
    {
        if (dirLight)
        {
            DirectionalShadowMapPass(dirLight, scene);
        }
    }

    for (auto* pointLight : lightData.pointLights)
    {
        if (pointLight)
        {
            OmniDirectionalShadowMapPass(pointLight, scene);
        }
    }
    
    if (antialiasingData.enabled)
    {
        glBindFramebuffer(GL_FRAMEBUFFER, antialiasingData.FBO);
    }
    else
    {
        glBindFramebuffer(GL_FRAMEBUFFER, screenRenderData.postProcessingFBO);
    }
 
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    // temporally enable stencil mask to clear it
    glStencilMask(0xFF);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    glStencilMask(0x00);

    const auto projection = scene->GetProjectionMatrix();
    RenderPass(projection, scene);
    
    if (antialiasingData.enabled)
    {
        const auto window = App::GetCurrentWindow();
        const auto bufferWidth = window->GetBufferWidth();
        const auto bufferHeight = window->GetBufferHeight();
    
        glBindFramebuffer(GL_READ_FRAMEBUFFER, antialiasingData.FBO);
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, screenRenderData.postProcessingFBO);
        glBlitFramebuffer(0, 0, bufferWidth, bufferHeight, 0, 0, bufferWidth, bufferHeight, GL_COLOR_BUFFER_BIT, GL_NEAREST);
    }
    
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    sceneShaders.screenShader->Bind();

    glBindVertexArray(screenRenderData.quadVAO);
    glDisable(GL_DEPTH_TEST);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, screenRenderData.postProcessingTexture);
    glDrawArrays(GL_TRIANGLES, 0, 6);

    sceneShaders.screenShader->Unbind();
}

void OpenGLRenderer::InitSceneShaders()
{
    ShaderData meshShaderData;
    meshShaderData.vertShader = ShaderData::shadersFolder + "mesh.vert";
    meshShaderData.fragShader = ShaderData::shadersFolder + "mesh.frag";
    sceneShaders.meshShader = CREATE_SHADER(meshShaderData);
    
    ShaderData directionalShadowMapShaderData;
    directionalShadowMapShaderData.vertShader = ShaderData::shadersFolder + "directional_shadow_map.vert";
    directionalShadowMapShaderData.fragShader = ShaderData::shadersFolder + "directional_shadow_map.frag";
    sceneShaders.directionalShadowMapShader = CREATE_SHADER(directionalShadowMapShaderData);
    
    ShaderData omniShadowMapShaderData;
    omniShadowMapShaderData.vertShader = ShaderData::shadersFolder + "omni_shadow_map.vert";
    omniShadowMapShaderData.fragShader = ShaderData::shadersFolder + "omni_shadow_map.frag";
    sceneShaders.omniShadowMapShader = CREATE_SHADER(omniShadowMapShaderData);
    
    ShaderData screenShaderData;
    screenShaderData.vertShader = ShaderData::shadersFolder + "screen.vert";
    screenShaderData.fragShader = ShaderData::shadersFolder + "screen.frag";
    sceneShaders.screenShader = CREATE_SHADER(screenShaderData);
}

void OpenGLRenderer::Render(const std::shared_ptr<const Shader>& shader, const Scene* scene, bool visualPass) const
{
    const auto& renderData = scene->GetRenderData();
    for (auto* renderComponent : renderData.renderComponents)
    {
        renderComponent->Draw(shader, visualPass);
    }
}

void OpenGLRenderer::DirectionalShadowMapPass(const DirectionalLightObject* dirLightComp, const Scene* scene) const
{
    sceneShaders.directionalShadowMapShader->Bind();

    auto shadowMap = dirLightComp->GetShadowMap();
    glViewport(0, 0, shadowMap->GetWidth(), shadowMap->GetHeight());
    shadowMap->Write();
    glClear(GL_DEPTH_BUFFER_BIT);

    sceneShaders.directionalShadowMapShader->SetMat4f(UniformNames::directionalLightProjection, dirLightComp->GetViewProjectionMatrix());

    sceneShaders.directionalShadowMapShader->Validate();
    Render(sceneShaders.directionalShadowMapShader, scene, false);

    sceneShaders.directionalShadowMapShader->Unbind();
}

void OpenGLRenderer::OmniDirectionalShadowMapPass(const PointLightObject* pointLightComp, const Scene* scene) const
{
    sceneShaders.omniShadowMapShader->Bind();

    auto shadowMap = pointLightComp->GetShadowMap();
    glViewport(0, 0, shadowMap->GetWidth(), shadowMap->GetHeight());
    shadowMap->Write();
    glClear(GL_DEPTH_BUFFER_BIT);

    sceneShaders.omniShadowMapShader->SetVec3f(UniformNames::lightPos, pointLightComp->GetPosition());
    sceneShaders.omniShadowMapShader->SetFloat("farPlane", 100.0f);
    
    const auto& lightMatrices = pointLightComp->GetLightMatrices();
    for (size_t i = 0; i < 6; i++)
    {
        const std::string uniformName = "lightMatrices[" + std::to_string(i) + "]";
        sceneShaders.omniShadowMapShader->SetMat4f(uniformName, lightMatrices[i]);
    }
    
    sceneShaders.omniShadowMapShader->Validate();
    Render(sceneShaders.omniShadowMapShader, scene, false);

    sceneShaders.omniShadowMapShader->Unbind();
}

void OpenGLRenderer::RenderPass(const glm::mat4& projection, const Scene* scene) const
{
    const auto window = App::GetCurrentWindow();
    glViewport(0, 0, window->GetBufferWidth(), window->GetBufferHeight());

    auto viewMatrix = scene->GetActiveCamera()->GetViewMatrix();

    const auto& lightData = scene->GetLightsData();
    
    sceneShaders.meshShader->Bind();

    for (auto* dirLight : lightData.dirLights)
    {
        if (dirLight)
        {
            dirLight->Use(sceneShaders.meshShader);
        }
    }

    for (auto* pointLight : lightData.pointLights)
    {
        if (pointLight)
        {
            pointLight->Use(sceneShaders.meshShader);
        }
    }

    sceneShaders.meshShader->SetMat4f(UniformNames::projection, projection);
    sceneShaders.meshShader->SetMat4f(UniformNames::view, viewMatrix);

    sceneShaders.meshShader->SetVec3f(UniformNames::cameraPosition, scene->GetActiveCamera()->GetPosition());

    for (size_t i = 0; i < SceneLightsData::maxLights; i++)
    {
        const std::string shadowMapName = "dirShadowMaps[" + std::to_string(i) + "]";
        sceneShaders.meshShader->SetInt(shadowMapName, static_cast<int>(i+1));
        
        if (i < DirectionalLightObject::lightCount)
        {
            const auto* dirLight = lightData.dirLights[i];
            dirLight->GetShadowMap()->Read(GL_TEXTURE1 + static_cast<int>(i));
    
            sceneShaders.meshShader->SetMat4f(UniformNames::directionalLightProjection, dirLight->GetViewProjectionMatrix());
        }
    }

    for (size_t i = SceneLightsData::maxLights; i < 2 * SceneLightsData::maxLights; i++)
    {
        const std::string shadowMapName = "omniShadowMaps[" + std::to_string(i - 10) + "].shadowMap";
        sceneShaders.meshShader->SetInt(shadowMapName, static_cast<int>(i+1));
    
        if (i < PointLightObject::lightCount + SceneLightsData::maxLights)
        {
            const auto* pointLight = lightData.pointLights[i - SceneLightsData::maxLights];
            pointLight->GetShadowMap()->Read(GL_TEXTURE1 + static_cast<int>(i));
        }
    }

    sceneShaders.meshShader->Validate();
    Render(sceneShaders.meshShader, scene, true);

    auto* skybox = scene->GetSkybox();
    if (skybox)
    {
        skybox->Draw(projection, viewMatrix);
    }

    sceneShaders.meshShader->Unbind();
}

void OpenGLRenderer::ResetRenderBuffer()
{
    if (antialiasingData.FBO != 0)
    {
        glDeleteFramebuffers(1, &antialiasingData.FBO);
        antialiasingData.FBO = 0;
    }

    if (antialiasingData.colorbufferTexture != 0)
    {
        glDeleteTextures(1, &antialiasingData.colorbufferTexture);
        antialiasingData.colorbufferTexture = 0;
    }

    if (antialiasingData.RBO != 0)
    {
        glDeleteRenderbuffers(1, &antialiasingData.RBO);
        antialiasingData.RBO = 0;
    }

    if (screenRenderData.quadVAO != 0)
    {
        glDeleteVertexArrays(1, &screenRenderData.quadVAO);
        screenRenderData.quadVAO = 0;
    }

    if (screenRenderData.quadVBO != 0)
    {
        glDeleteBuffers(1, &screenRenderData.quadVBO);
        screenRenderData.quadVBO = 0;   
    }
    
    if (screenRenderData.postProcessingFBO != 0)
    {
        glDeleteFramebuffers(1, &screenRenderData.postProcessingFBO);
        screenRenderData.postProcessingFBO = 0;
    }
    
    if (screenRenderData.postProcessingRBO != 0)
    {
        glDeleteRenderbuffers(1, &screenRenderData.postProcessingRBO);
        screenRenderData.postProcessingRBO = 0;
    }
    
    if (screenRenderData.postProcessingTexture != 0)
    {
        glDeleteTextures(1, &screenRenderData.postProcessingTexture);
        screenRenderData.postProcessingTexture = 0;
    }
}

void OpenGLRenderer::OnWindowSizeChanged(Window* window, int bufferWidth, int bufferHeight)
{
    InitRenderBuffer(bufferWidth, bufferHeight);
}
