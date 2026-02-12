#pragma once

#include "Rendering/Core/Material.h"

struct TextureData;
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
struct Transform;

enum class RendererType
{
    OpenGL
};

class Renderer
{
public:
    virtual ~Renderer() {}
    
    static Renderer* Get() { return rendererAPI.get(); }
    
    static void Init(RendererType type);
    
    virtual TexturePtr CreateTexture(const std::string& fileLocation) const = 0;
    virtual TexturePtr CreateTexture(const TextureData& textureData) const = 0;
    
    virtual ShaderPtr CreateShader(const ShaderData& shaderData) const = 0;
    virtual MeshPtr CreateMesh(const MeshData& meshData) const = 0;
    virtual InstancedMeshPtr CreateInstancedMesh(MeshPtr mesh, const std::vector<Transform>& transforms) const = 0;
    virtual LinePtr CreateLine() const = 0;
    
    virtual SkyboxPtr CreateSkybox(const std::vector<std::string>& faceLocations) const { return nullptr; }
    
    virtual ShadowMapPtr CreateShadowMap(int width, int height) const { return nullptr; }
    virtual ShadowMapPtr CreateOmniShadowMap(int width, int height) const { return nullptr; }
    
    virtual MaterialPtr CreateRefractMaterial(const MaterialResources& resources) const { return nullptr; }
    virtual MaterialPtr CreateReflectMaterial(const MaterialResources& resources) const { return nullptr; }
    
    virtual void RenderScene(const Scene* scene) const = 0;
    virtual void InitSceneShaders() = 0;
    
protected:
    static inline RendererPtr rendererAPI;
};