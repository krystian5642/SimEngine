#pragma once

#include "Rendering/Renderer/Renderer.h"

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
    std::shared_ptr<Texture> CreateTexture(unsigned char* data, int width, int height) const override;
    std::shared_ptr<Shader> CreateShader(const ShaderData& shaderData) const override;
    std::shared_ptr<Mesh> CreateMesh(const MeshData& meshData) const override;
    std::unique_ptr<Line> CreateLine() const override;
    
    std::unique_ptr<Skybox> CreateSkybox(const std::vector<std::string>& faceLocations) const override;
    
    std::shared_ptr<ShadowMap> CreateShadowMap(int width, int height) const override;
    std::shared_ptr<ShadowMap> CreateOmniShadowMap(int width, int height) const override;
    
    std::shared_ptr<Material> CreateRefractMaterial(const MaterialResources& resources) const;
    std::shared_ptr<Material> CreateReflectMaterial(const MaterialResources& resources) const;
    
private:
    void InitRenderBuffer(int bufferWidth, int bufferHeight);
    
    void RenderScene(const Scene* scene) const override;
    void InitSceneShaders() override;
    
    void Render(const std::shared_ptr<const Shader>& shader, const Scene* scene, bool visualPass) const;
    
    // Render passes
    void DirectionalShadowMapPass(const DirectionalLightObject* dirLightComp, const Scene* scene) const;
    void OmniDirectionalShadowMapPass(const PointLightObject* pointLightComp, const Scene* scene) const;
    void RenderPass(const glm::mat4& projection, const Scene* scene) const;
    
    void ResetRenderBuffer();
    
    void OnWindowSizeChanged(Window* window, int bufferWidth, int bufferHeight);
    
    GLSceneShaders sceneShaders;
    GLScreenRenderData screenRenderData;
};
