
#include "DefaultScene.h"

#include "Core/App.h"
#include "Objects/Entities/CameraEntity.h"
#include "Objects/Lighting/DirectionalLightObject.h"

DefaultScene::DefaultScene(const std::string& name)
    : Scene(name)
{
    App::Get().renderer.clearColor = {0.2f, 0.2f, 0.2f}; 
    
    camera = AddObject<CameraEntity>("Camera")->GetCameraComponent();
    camera->SetAsActiveCamera();
    
    auto light = AddObject<DirectionalLightObject>("Directional Light");
    light->SetDirection({0.1f, -60.0f, 0.1f});
    light->lightData.ambientIntensity = 0.5f;
    light->lightData.diffuseIntensity = 0.8f;
}
