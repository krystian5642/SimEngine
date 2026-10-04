#include "TestScenes.h"

#include "Components/CameraComponent.h"
#include "Managers/MaterialManager.h"
#include "Managers/MeshManager.h"
#include "Physics/RigidBody/Components/RigidBodyComponent.h"
#include "Physics/RigidBody/Entities/RigidBody.h"
#include "Scene/Objects/Entities/MeshEntity.h"
#include "Physics/RigidBody/Components/ShapeComponents.h"

RigidBodyTestScene::RigidBodyTestScene(const std::string& name)
    : DefaultScene(name)
{
    rigidBody = AddObject<RigidBody>("Rigid Body");
    //rigidBody->rigidBodyComponent->forceAtLocationAffectsLinearMotion = false;
    
    camera->SetPosition({-1.5f, 4.7f, 8.7f});
    camera->SetRotation(-27.0f, 14.0f);
    
    applyTorque.Set([this]()
    {
        rigidBody->ApplyForceAtLocation({-0.8f, 0.2f, 0.1f}
            , {0.4f, 0.0f, 0.1f});
        
        const auto& properties 
            = rigidBody->GetShapeComponent()->GetProperties();
        for (auto* property : properties)
        {
            property->readOnly = true;
        }
    });
    
    auto plane = AddObject<MeshEntity>();
    
    plane->meshComponent->mesh = MeshManager::Get().GetAssetByName("plane");
    plane->meshComponent->material = MaterialManager::Get().GetAssetByName("chrome");
    
    plane->SetScale({7.0f, 1.0f, 7.0f});
    plane->Move({0.0f, -2.0f, 0.0f});
}
