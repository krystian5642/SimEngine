#pragma once

#include "Rendering/Renderer/Renderer.h"

class GLShader;
class DirectionalLightObject;
class PointLightObject;

using ConstGLShaderPtr = std::shared_ptr<const GLShader>;

struct GLSceneShaders
{
    ConstGLShaderPtr meshShader;
    ConstGLShaderPtr directionalShadowMapShader;
    ConstGLShaderPtr omniShadowMapShader;
    ConstGLShaderPtr screenShader;
    ConstGLShaderPtr FXAAshader;
};

struct GLScreenRenderData
{
    GLuint quadVAO{};
    GLuint quadVBO{};
    
    GLuint postProcessingFBO{};
    GLuint postProcessingRBO{};
    GLuint postProcessingTexture{};
    
    void Reset();
};

struct GLAntialiasingData
{
    GLuint FBO{};
    GLuint RBO{};
    GLuint colorbufferTexture{};
    
    AntialiasingMethod method{AntialiasingMethod::MSAA};
    
    void Reset();
};

class OpenGLRenderer : public Renderer
{
public:
    OpenGLRenderer();
    ~OpenGLRenderer() override;
    
    void SetAntialiasingMethod(AntialiasingMethod antialiasingMethod) override;
    AntialiasingMethod GetAntialiasingMethod() const override;
    
    void SetFXAASettings(const FXAASettings& settings) override;
    const FXAASettings& GetFXAASettings() const override;
    
    void SetMSAASettings(const MSAASettings& settings) override;
    const MSAASettings& GetMSAASettings() const override;
    
    unsigned int GetMaxSamples() const override;
    
    TexturePtr CreateTexture(const std::string& fileLocation) const override;
    TexturePtr CreateTexture(const TextureData& textureData) const override;
    ShaderPtr CreateShader(const ShaderData& shaderData) const override;
    MeshPtr CreateMesh(const MeshData& meshData) const override;
    InstancedMeshPtr CreateInstancedMesh(MeshPtr mesh, const std::vector<Transform>& transforms) const override;
    LinePtr CreateLine() const override;
    
    SkyboxPtr CreateSkybox(const std::vector<std::string>& faceLocations) const override;
    
    ShadowMapPtr CreateShadowMap(int width, int height) const override;
    ShadowMapPtr CreateOmniShadowMap(int width, int height) const override;
    
    MaterialPtr CreateRefractMaterial(const MaterialResources& resources) const override;
    MaterialPtr CreateReflectMaterial(const MaterialResources& resources) const override;
    
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
    GLAntialiasingData antialiasingData;
    
    FXAASettings fxaaSettings;
    MSAASettings msaaSettings;
};
