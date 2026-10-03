#include "TestScenes.h"

#include "Components/CameraComponent.h"
#include "Physics/RigidBody/Entities/RigidBody.h"

RigidBodyTestScene::RigidBodyTestScene(const std::string& name)
    : DefaultScene(name)
{
    rigidBody = AddObject<RigidBody>("Rigid Body");
    
    camera->SetPosition({-1.5f, 4.7f, 8.7f});
    camera->SetRotation(-27.0f, 14.0f);
    
    applyTorque.Set([this]()
    {
        rigidBody->ApplyTorque({-0.3f, 0.3f, 0.6f}
        , {0.3f, 0.0f, 0.0f});
    });
}