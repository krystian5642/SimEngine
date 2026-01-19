#pragma once

class Window;
class Scene;

class SceneManager
{
    using SceneFunc = std::unique_ptr<Scene> (*)();
public:
    static Scene* GetCurrentScene() { return currentScene.get(); }
    
    static void SetDefaultScene(const std::string& sceneName) { defaultSceneName = sceneName; }
    
    static void LoadDefaultScene();
    static void RegisterScene(const std::string& sceneName, SceneFunc sceneFunc);
    static void LoadScene(const std::string& sceneName);
    static void Init();
    static void Tick(float deltaTime);
    static void OnDestroy();
    static void Render();
    static std::vector<std::string> GetSceneNames();
    
protected:
    static inline std::unique_ptr<Scene> currentScene;
    static inline std::string defaultSceneName = "DefaultScene";
    
    static inline std::unordered_map<std::string, SceneFunc> loadSceneFuncs;
};
