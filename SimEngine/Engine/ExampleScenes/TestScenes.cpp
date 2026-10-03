#include "TestScenes.h"

#include "imgui.h"
#include "Components/CameraComponent.h"
#include "Physics/RigidBody/Entities/RigidBody.h"

RigidBodyTestScene::RigidBodyTestScene(const std::string& name)
    : DefaultScene(name)
{
    rigidBody = AddObject<RigidBody>("Rigid Body");
    
    camera->SetPosition({-1.5f, 4.7f, 8.7f});
    camera->SetRotation(-27.0f, 14.0f);
}

void RigidBodyTestScene::Tick(float deltaTime)
{
    DefaultScene::Tick(deltaTime);
}

void RigidBodyTestScene::DrawUI()
{
    DefaultScene::DrawUI();
    
    if (ImGui::Button("Apply Force"))
    {
        rigidBody->ApplyTorque({0.0f, 10.0f, 2.4f}
            , {2.0f, 0.0f, 0.0f});
    }
}
