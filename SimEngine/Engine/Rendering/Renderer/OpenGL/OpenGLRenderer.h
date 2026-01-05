#pragma once

#include "Rendering/Renderer/Renderer.h"

namespace SimEngine
{
    class GLShader;
    class DirectionalLightObject;
    class PointLightObject;
    
    struct GLSceneShaders
    {
        std::shared_ptr<const GLShader> meshShader;
        std::shared_ptr<const GLShader> directionalShadowMapShader;
        std::shared_ptr<const GLShader> omniShadowMapShader;
        std::shared_ptr<const GLShader> screenShader;
    };
    
    struct GLScreenRenderData
    {
        GLuint FBO{};
        GLuint RBO{};
        GLuint colorbufferTexture{};
        GLuint quadVAO{};
        GLuint quadVBO{};
    };
    
    class OpenGLRenderer : public Renderer
    {
    public:
        OpenGLRenderer();
        ~OpenGLRenderer() override;
        
    protected:
        std::shared_ptr<Texture> CreateTexture(const std::string& fileLocation) const override;
        std::shared_ptr<Shader> CreateShader(const ShaderData& shaderData) const override;
        std::shared_ptr<Mesh> CreateMesh(const MeshData& meshData) const override;
        
        std::shared_ptr<Skybox> CreateSkybox(const std::vector<std::string>& faceLocations) const override;
        
        std::shared_ptr<ShadowMap> CreateShadowMap(int width, int height) const override;
        std::shared_ptr<ShadowMap> CreateOmniShadowMap(int width, int height) const override;
        
    private:
        void InitSceneShaders();
        
        void RenderScene(const Scene* scene, const Window& window) const override;
        
        void Render(const std::shared_ptr<const Shader>& shader, const Scene* scene) const;
        
        // Render passes
        void DirectionalShadowMapPass(const DirectionalLightObject* dirLightComp, const Scene* scene) const;
        void OmniDirectionalShadowMapPass(const PointLightObject* pointLightComp, const Scene* scene) const;
        void RenderPass(const glm::mat4& projection, const Scene* scene) const;
        
        GLSceneShaders sceneShaders;
        GLScreenRenderData screenRenderData;
    };
}
