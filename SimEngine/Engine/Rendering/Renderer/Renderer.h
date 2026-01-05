#pragma once

namespace SimEngine
{
    class Window;
    class Scene;
    struct MaterialResources;
    struct ShaderData;
    struct MeshData;
    class Shader;
    class Skybox;
    class Texture;
    class Material;
    class Mesh;
    class ShadowMap;
    
    class Renderer
    {
    public:
        Renderer() = default;
        virtual ~Renderer() {}
        
        static void InitStatic(std::unique_ptr<Renderer> renderer)
        {
            rendererAPI = std::move(renderer);
        }
        
        static void RenderSceneStatic(const Scene* scene, const Window& window)
        {
            rendererAPI->RenderScene(scene, window);
        }
        
        static std::shared_ptr<Texture> CreateTextureStatic(const std::string& fileLocation)
        {
            return rendererAPI->CreateTexture(fileLocation);
        }
        
        static std::shared_ptr<Shader> CreateShaderStatic(const ShaderData& shaderData)
        {
            return rendererAPI->CreateShader(shaderData);
        }
        
        static std::shared_ptr<Mesh> CreateMeshStatic(const MeshData& meshData)
        {
            return rendererAPI->CreateMesh(meshData);
        }
        
        static std::shared_ptr<Skybox> CreateSkyboxStatic(const std::vector<std::string>& faceLocations)
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
        
    protected:
        static inline std::unique_ptr<Renderer> rendererAPI;
        
    protected:
        virtual void RenderScene(const Scene* scene, const Window& window) const = 0;
        
        virtual std::shared_ptr<Texture> CreateTexture(const std::string& fileLocation) const = 0;
        virtual std::shared_ptr<Shader> CreateShader(const ShaderData& shaderData) const = 0;
        virtual std::shared_ptr<Mesh> CreateMesh(const MeshData& meshData) const = 0;
    
        virtual std::shared_ptr<Skybox> CreateSkybox(const std::vector<std::string>& faceLocations) const { return nullptr; }
        
        virtual std::shared_ptr<ShadowMap> CreateShadowMap(int width, int height) const { return nullptr; }
        virtual std::shared_ptr<ShadowMap> CreateOmniShadowMap(int width, int height) const { return nullptr; }
    };
}
