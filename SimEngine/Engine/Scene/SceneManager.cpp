#include "SceneManager.h"

#include "Scene.h"

namespace SimEngine
{
    void SceneManager::LoadDefaultScene()
    {
        LoadScene(defaultSceneName);
    }
        
    void SceneManager::RegisterScene(const std::string& sceneName, SceneFunc sceneFunc)
    {
        loadSceneFuncs[sceneName] = sceneFunc;
    }
    
    void SceneManager::LoadScene(const std::string& sceneName)
    {
        if (currentScene)
        {
            currentScene->OnDestroy();
        }
            
        currentScene = loadSceneFuncs[sceneName]();
            
        currentScene->Init();
        currentScene->Start();
    }
        
    void SceneManager::Init()
    {
        LoadDefaultScene();
    }
        
    void SceneManager::Tick(float deltaTime)
    {
        currentScene->Tick(deltaTime);
    }
        
    void SceneManager::OnDestroy()
    {
        currentScene->OnDestroy();
    }
        
    void SceneManager::Render(const Window& window)
    {
        currentScene->Render(window);
    }
}
