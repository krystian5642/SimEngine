#pragma once

#include "Rendering/Core/Material.h"

enum class TextureFormat;
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

enum class AntialiasingMethod
{
    None,
    FXAA,
    MSAA
};

struct FXAASettings
{
    float FXAASpanMax{8.0f};
    float FXAAReduceMin{1.0f / 128.0f};
    float FXAAReduceMul{1.0f / 8.0f};
};

struct MSAASettings
{
    unsigned int samples{4};
};

namespace UniformNames
{
    UNIFORM_NAME texelSize = "texelSize";
    UNIFORM_NAME fXAASpanMax = "fxaaSettings.FXAASpanMax";
    UNIFORM_NAME fXAAReduceMin = "fxaaSettings.FXAAReduceMin";
    UNIFORM_NAME fXAAReduceMul = "fxaaSettings.FXAAReduceMul";
}

enum class RenderPolygonMode
{
    None,
    Fill,
    Line,
    Point
};

class Renderer
{
public:
    virtual ~Renderer() {}
    
    static Renderer* Get() { return rendererAPI.get(); }
    
    static void Init(RendererType type);
    
    virtual void SetAntialiasingMethod(AntialiasingMethod antialiasingMethod) {}
    virtual AntialiasingMethod GetAntialiasingMethod() const { return AntialiasingMethod::None; }
    
    virtual void SetFXAASettings(const FXAASettings& settings) {}
    virtual const FXAASettings& GetFXAASettings() const { return {}; }
    
    virtual void SetMSAASettings(const MSAASettings& settings) {}
    virtual const MSAASettings& GetMSAASettings() const { return {}; }
    
    virtual unsigned int GetMaxSamples() const { return 0; }
    
    virtual void SetRenderPolygonMode(RenderPolygonMode mode) {}
    virtual RenderPolygonMode GetRenderPolygonMode() const { return RenderPolygonMode::None; }
    
    virtual void SetLineWidth(float width) {}
    virtual float GetLineWidth() const { return 0.0f; }
    
    virtual void SetPointSize(float size) {}
    virtual float GetPointSize() const { return 0.0f; }
    
    virtual TexturePtr CreateTexture(const std::string& fileLocation, TextureFormat format) const = 0;
    virtual TexturePtr CreateTexture(const TextureData& textureData, TextureFormat format) const = 0;
    
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