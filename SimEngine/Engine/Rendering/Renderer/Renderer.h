#pragma once

#include "Rendering/Core/Line.h"
#include "Rendering/Core/Skybox.h"
#include "Rendering/Core/Material.h"

class Window;
class Scene;
struct MaterialResources;
struct ShaderData;
struct MeshData;
class Shader;
class Texture;
class Material;
class Mesh;
class ShadowMap;

class Renderer
{
public:
    virtual ~Renderer() {}
    
    static void InitStatic(std::unique_ptr<Renderer> renderer)
    {
        rendererAPI = std::move(renderer);
    }
    
    static void RenderSceneStatic(const Scene* scene)
    {
        rendererAPI->RenderScene(scene);
    }
    
    static void InitSceneShadersStatic()
    {
        rendererAPI->InitSceneShaders();
    }
    
    static std::shared_ptr<Texture> CreateTextureStatic(const std::string& fileLocation)
    {
        return rendererAPI->CreateTexture(fileLocation);
    }
    
    static std::shared_ptr<Texture> CreateTextureStatic(unsigned char* data, int width, int height)
    {
        return rendererAPI->CreateTexture(data, width, height);
    }
    
    static std::shared_ptr<Shader> CreateShaderStatic(const ShaderData& shaderData)
    {
        return rendererAPI->CreateShader(shaderData);
    }
    
    static std::shared_ptr<Mesh> CreateMeshStatic(const MeshData& meshData)
    {
        return rendererAPI->CreateMesh(meshData);
    }
    
    static std::unique_ptr<Line> CreateLineStatic()
    {
        return rendererAPI->CreateLine();
    }
    
    static std::unique_ptr<Skybox> CreateSkyboxStatic(const std::vector<std::string>& faceLocations)
    {
        return rendererAPI->CreateSkybox(faceLocations);
    }
    
    static std::shared_ptr<ShadowMap> CreateShadowMapStatic(int width, int height)
    {
        return rendererAPI->CreateShadowMap(width, height);
    }
    
    static std::shared_ptr<ShadowMap> CreateOmniShadowMapStatic(int width, int height)
    {
        return rendererAPI->CreateOmniShadowMap(width, height);
    }
    
    static std::shared_ptr<Material> CreateRefractMaterialStatic()
    {
        ShaderData shaderData;
        shaderData.fragShader = ShaderData::shadersFolder + "refract.frag";
        shaderData.vertShader = ShaderData::shadersFolder + "refract.vert";
        
        MaterialResources resources;
        resources.shader = rendererAPI->CreateShader(shaderData);
        
        return rendererAPI->CreateRefractMaterial(resources);
    }
    
    static std::shared_ptr<Material> CreateReflectMaterialStatic()
    {
        ShaderData shaderData;
        shaderData.fragShader = ShaderData::shadersFolder + "reflect.frag";
        shaderData.vertShader = ShaderData::shadersFolder + "reflect.vert";
        
        MaterialResources resources;
        resources.shader = rendererAPI->CreateShader(shaderData);
        
        return rendererAPI->CreateReflectMaterial(resources);
    }
    
protected:
    static inline std::unique_ptr<Renderer> rendererAPI;
    
protected:
    virtual void RenderScene(const Scene* scene) const = 0;
    virtual void InitSceneShaders() = 0;
    
    virtual std::shared_ptr<Texture> CreateTexture(const std::string& fileLocation) const = 0;
    virtual std::shared_ptr<Texture> CreateTexture(unsigned char* data, int width, int height) const = 0;
    virtual std::shared_ptr<Shader> CreateShader(const ShaderData& shaderData) const = 0;
    virtual std::shared_ptr<Mesh> CreateMesh(const MeshData& meshData) const = 0;
    virtual std::unique_ptr<Line> CreateLine() const = 0;
    
    virtual std::unique_ptr<Skybox> CreateSkybox(const std::vector<std::string>& faceLocations) const { return nullptr; }
    
    virtual std::shared_ptr<ShadowMap> CreateShadowMap(int width, int height) const { return nullptr; }
    virtual std::shared_ptr<ShadowMap> CreateOmniShadowMap(int width, int height) const { return nullptr; }
    
    virtual std::shared_ptr<Material> CreateRefractMaterial(const MaterialResources& resources) const { return nullptr; }
    virtual std::shared_ptr<Material> CreateReflectMaterial(const MaterialResources& resources) const { return nullptr; }
};
