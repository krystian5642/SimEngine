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

#define CREATE_SHADER(shaderData) std::dynamic_pointer_cast<const GLShader>(CreateShader(shaderData))

namespace SimEngine
{
    OpenGLRenderer::OpenGLRenderer()
    {
        glewExperimental = GL_TRUE;
        if (glewInit() != GLEW_OK)
        {
            glfwDestroyWindow(App::GetCurrentWindow()->GetGLFWWindow());
            glfwTerminate();
            throw std::runtime_error("Failed to initialize GLEW");
        }
        
        InitSceneShaders();
        
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_CULL_FACE);
        
        glGenFramebuffers(1, &screenRenderData.FBO);
        glBindFramebuffer(GL_FRAMEBUFFER, screenRenderData.FBO);
        
        glGenTextures(1, &screenRenderData.colorbufferTexture);
        glBindTexture(GL_TEXTURE_2D, screenRenderData.colorbufferTexture);
        //glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, renderData.colorbufferTexture);
        //glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, 4, GL_RGB, 1920, 1080, GL_LINEAR);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 1920, 1080, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
        //glTexParameteri(GL_TEXTURE_2D_MULTISAMPLE, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        
        glBindTexture(GL_TEXTURE_2D, 0);
        //glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);
        //glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, renderData.colorbufferTexture, 0); 
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, screenRenderData.colorbufferTexture, 0);
        
        glGenRenderbuffers(1, &screenRenderData.RBO);
        glBindRenderbuffer(GL_RENDERBUFFER, screenRenderData.RBO);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, 1920, 1080);
        //glRenderbufferStorageMultisample(GL_RENDERBUFFER, 4, GL_DEPTH24_STENCIL8, 1920, 1080);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, screenRenderData.RBO);
        
        GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
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

    OpenGLRenderer::~OpenGLRenderer()
    {
        if (screenRenderData.FBO != 0)
        {
            glDeleteFramebuffers(1, &screenRenderData.FBO);
        }
    
        if (screenRenderData.colorbufferTexture != 0)
        {
            glDeleteTextures(1, &screenRenderData.colorbufferTexture);
        }
    
        if (screenRenderData.RBO != 0)
        {
            glDeleteRenderbuffers(1, &screenRenderData.RBO);
        }
    
        if (screenRenderData.quadVAO != 0)
        {
            glDeleteVertexArrays(1, &screenRenderData.quadVAO);
        }
    
        if (screenRenderData.quadVBO != 0)
        {
            glDeleteBuffers(1, &screenRenderData.quadVBO);
        }
    }

    std::shared_ptr<Texture> OpenGLRenderer::CreateTexture(const std::string& fileLocation) const
    {
        return std::make_shared<GLTexture>(fileLocation);
    }

    std::shared_ptr<Shader> OpenGLRenderer::CreateShader(const ShaderData& shaderData) const
    {
        return std::make_shared<GLShader>(shaderData);
    }

    std::shared_ptr<Mesh> OpenGLRenderer::CreateMesh(const MeshData& meshData) const
    {
        return std::make_shared<GLMesh>(meshData);
    }
    
    std::shared_ptr<Skybox> OpenGLRenderer::CreateSkybox(const std::vector<std::string>& faceLocations) const
    {
        return std::make_shared<GLSkybox>(faceLocations);
    }
    
    std::shared_ptr<ShadowMap> OpenGLRenderer::CreateShadowMap(int width, int height) const
    {
        return std::make_shared<GLShadowMap>(width, height);
    }

    std::shared_ptr<ShadowMap> OpenGLRenderer::CreateOmniShadowMap(int width, int height) const
    {
        return std::make_shared<GLOmniShadowMap>(width, height);
    }

    void OpenGLRenderer::InitSceneShaders()
    {
        const std::string shadersFolder = "Engine/Rendering/Shaders/";
        
        ShaderData meshShaderData;
        meshShaderData.vertShader = shadersFolder + "main_shader.vert";
        meshShaderData.fragShader = shadersFolder + "main_shader.frag";
        sceneShaders.meshShader = CREATE_SHADER(meshShaderData);
        
        ShaderData directionalShadowMapShaderData;
        directionalShadowMapShaderData.vertShader = shadersFolder + "directional_shadow_map.vert";
        directionalShadowMapShaderData.fragShader = shadersFolder + "directional_shadow_map.frag";
        sceneShaders.directionalShadowMapShader = CREATE_SHADER(directionalShadowMapShaderData);
        
        ShaderData omniShadowMapShaderData;
        omniShadowMapShaderData.vertShader = shadersFolder + "omni_shadow_map.vert";
        omniShadowMapShaderData.fragShader = shadersFolder + "omni_shadow_map.frag";
        sceneShaders.omniShadowMapShader = CREATE_SHADER(omniShadowMapShaderData);
        
        ShaderData screenShaderData;
        screenShaderData.vertShader = shadersFolder + "screen.vert";
        screenShaderData.fragShader = shadersFolder + "screen.frag";
        sceneShaders.screenShader = CREATE_SHADER(screenShaderData);
    }

    void OpenGLRenderer::RenderScene(const Scene* scene, const Window& window) const
    {
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
    
        glBindFramebuffer(GL_FRAMEBUFFER, screenRenderData.FBO);
        glEnable(GL_DEPTH_TEST);
    
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    
        // temporally enable stencil mask to clear it
        glStencilMask(0xFF);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
        glStencilMask(0x00);
    
        const glm::mat4 projection = glm::perspective(glm::radians(45.0f), window.GetAspectRatio(), 0.1f, 10000.0f);
        RenderPass(projection, scene);
    
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
    
        sceneShaders.screenShader->Bind();

        glBindVertexArray(screenRenderData.quadVAO);
        glDisable(GL_DEPTH_TEST);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, screenRenderData.colorbufferTexture);
        glDrawArrays(GL_TRIANGLES, 0, 6);
    
        sceneShaders.screenShader->Unbind();
    }

    void OpenGLRenderer::Render(const std::shared_ptr<const Shader>& shader, const Scene* scene) const
    {
        const auto& renderData = scene->GetRenderData();
        for (auto* renderComponent : renderData.renderComponents)
        {
            renderComponent->Draw(shader);
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
        Render(sceneShaders.directionalShadowMapShader, scene);

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
        Render(sceneShaders.omniShadowMapShader, scene);
    
        sceneShaders.omniShadowMapShader->Unbind();
    }

    void OpenGLRenderer::RenderPass(const glm::mat4& projection, const Scene* scene) const
    {
        glViewport(0, 0, 1920, 1080);
    
        auto viewMatrix = scene->GetActiveCamera()->GetView();
    
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
            sceneShaders.meshShader->SetInt(shadowMapName, static_cast<int>(i));
            
            if (i < DirectionalLightObject::lightCount)
            {
                const auto* dirLight = lightData.dirLights[i];
                dirLight->GetShadowMap()->Read(GL_TEXTURE0 + static_cast<int>(i));
        
                sceneShaders.meshShader->SetMat4f(UniformNames::directionalLightProjection, dirLight->GetViewProjectionMatrix());
            }
        }
    
        for (size_t i = SceneLightsData::maxLights; i < 2 * SceneLightsData::maxLights; i++)
        {
            const std::string shadowMapName = "omniShadowMaps[" + std::to_string(i - 10) + "].shadowMap";
            sceneShaders.meshShader->SetInt(shadowMapName, static_cast<int>(i));
        
            if (i < PointLightObject::lightCount + SceneLightsData::maxLights)
            {
                const auto* pointLight = lightData.pointLights[i - SceneLightsData::maxLights];
                pointLight->GetShadowMap()->Read(GL_TEXTURE0 + static_cast<int>(i));
            }
        }

        sceneShaders.meshShader->Validate();
        Render(sceneShaders.meshShader, scene);

        auto* skybox = scene->GetSkybox();
        if (skybox)
        {
            skybox->Draw(projection, viewMatrix);
        }
    
        sceneShaders.meshShader->Unbind();
    }
}
