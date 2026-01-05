#include "ExampleApp.h"
#include "Scene/Scene.h"
#include "Scene/SceneManager.h"

ExampleApp::ExampleApp()
{
    auto defaultScene = []
    {
        auto scene = std::make_unique<SimEngine::Scene>();
        return scene;
    };
    
    SimEngine::SceneManager::RegisterScene("DefaultScene", defaultScene);
}

ExampleApp::~ExampleApp()
{
}
