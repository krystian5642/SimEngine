#pragma once

class Window;
class Scene;
class Shader;
class DirectionalLightObject;
class PointLightObject;

struct GLSceneShaders
{
    std::shared_ptr<const Shader> meshShader;
    std::shared_ptr<const Shader> screenShader;
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
    
    void Reset();
};

class Renderer
{
    friend class Scene;
    friend class SceneManager;
    
public:
    Renderer();
    ~Renderer();

    Renderer(const Renderer&) = delete;
    Renderer& operator=(const Renderer&) = delete;
    
    void Init();
    
    void SetLineWidth(float width);
    float GetLineWidth() const;
    
    void SetPointSize(float size);
    float GetPointSize() const;
    
    void EnableBlending(bool enable);
    
    const std::shared_ptr<const Shader>& GetBaseShader() const { return sceneShaders.meshShader; }
    
    glm::vec3 clearColor{0.0f, 0.0f, 0.0f};
    
private:
    void InitRenderBuffer(int bufferWidth, int bufferHeight);
    
    void RenderScene(const Scene* scene) const;
    void InitSceneShaders();
    
    void Render(const Scene* scene) const;
    
    void ResetRenderBuffer();
    
    void OnWindowSizeChanged(Window* window, int bufferWidth, int bufferHeight);
    
    GLSceneShaders sceneShaders;
    GLScreenRenderData screenRenderData;
    GLAntialiasingData antialiasingData;
};
