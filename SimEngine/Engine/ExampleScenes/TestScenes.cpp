#include "TestScenes.h"

#include "imgui.h"
#include "Physics/SphereComponent.h"
#include "Scene/Objects/Entities/Entity.h"
#include "Components/CameraComponent.h"

RigidBodyTestScene::RigidBodyTestScene(const std::string& name)
    : DefaultScene(name)
{
    rigidBody = AddObject<Entity>("Rigid Body");
    box = rigidBody->AddComponent<BoxComponent>();
    
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
    
    if (ImGui::Button("Apply Torque"))
    {
        box->ApplyTorque({0.0f, 1000 / 2.0f, 1000.0f}, {1.0f, 0.0f, 0.0f});
    }
}
