#include "SceneManager.h"

#include "Scene.h"
#include "Rendering/Renderer/Renderer.h"

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
        
    Renderer::InitSceneShadersStatic();
    
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
    
void SceneManager::Render()
{
    currentScene->Render();
}

std::vector<std::string> SceneManager::GetSceneNames()
{
    std::vector<std::string> keys;
    keys.reserve(loadSceneFuncs.size());
    for (const auto& func : loadSceneFuncs)
    {
        keys.push_back(func.first);
    }
    return keys;
}
