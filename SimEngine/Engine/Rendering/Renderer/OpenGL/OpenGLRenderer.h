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
};

struct GLScreenRenderData
{
    GLuint FBO{};
    GLuint RBO{};
    GLuint colorbufferTexture{};
    GLuint quadVAO{};
    GLuint quadVBO{};
    
    GLuint postProcessingFBO{};
    GLuint postProcessingTexture{};
};

class OpenGLRenderer : public Renderer
{
public:
    OpenGLRenderer();
    ~OpenGLRenderer() override;
    
protected:
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
    GLsizei antialiasingSamples{8};
};
