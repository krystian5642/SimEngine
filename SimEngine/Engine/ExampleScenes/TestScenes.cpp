#include "TestScenes.h"

#include "Components/CameraComponent.h"
#include "Managers/MaterialManager.h"
#include "Managers/MeshManager.h"
#include "Physics/RigidBody/Entities/RigidBody.h"
#include "Scene/Objects/Entities/MeshEntity.h"

RigidBodyTestScene::RigidBodyTestScene(const std::string& name)
    : DefaultScene(name)
{
    rigidBody = AddObject<RigidBody>("Rigid Body");
    
    camera->SetPosition({-1.5f, 4.7f, 8.7f});
    camera->SetRotation(-27.0f, 14.0f);
    
    applyTorque.Set([this]()
    {
        rigidBody->ApplyTorque({-0.8f, 0.2f, 0.1f}
        , {0.4f, 0.0f, 0.1f});
    });
    
    auto plane = AddObject<MeshEntity>();
    
    plane->meshComponent->mesh = MeshManager::Get().GetAssetByName("plane");
    plane->meshComponent->material = MaterialManager::Get().GetAssetByName("chrome");
    
    plane->SetScale({7.0f, 1.0f, 7.0f});
    plane->Move({0.0f, -2.0f, 0.0f});
}
