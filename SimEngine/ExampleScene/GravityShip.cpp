#include "GravityShip.h"

#include <GLFW/glfw3.h>

#include "GravityComponent.h"
#include "Components/InputComponent.h"
#include "Managers/MaterialManager.h"
#include "Managers/MeshManager.h"
#include "Scene/Objects/Entities/MeshEntity.h"

GravityShip::GravityShip(ObjectBase* parent, Scene* scene, const std::string& name)
    : Entity(parent, scene, name)
{
    float mass = 100000.0f;
    
    left = AddChild<MeshEntity>();
    auto grav = left->AddComponent<GravityComponent>();
    grav->gravityConstraints = {true, false, true};
    grav->gravityData.mass = mass;
    left->SetMesh(MeshManager::Get().GetAssetByName("sphere"));
    left->SetMaterial(MaterialManager::Get().GetAssetByName("chrome"));
    left->SetPosition({0.0f, 0.0f, -2.0f});
    left->SetScale({0.5f, 1.5f, 0.5f});
    
    right = AddChild<MeshEntity>();
    grav = right->AddComponent<GravityComponent>();
    grav->gravityConstraints = {true, false, true};
    grav->gravityData.mass = mass;
    right->SetMesh(MeshManager::Get().GetAssetByName("sphere"));
    right->SetMaterial(MaterialManager::Get().GetAssetByName("chrome"));
    right->SetPosition({4.0f, 0.0f, -2.0f});
    right->SetScale({0.5f, 1.5f, 0.5f});
    
    inputComponent = AddComponent<InputComponent>();
    inputComponent->tickWhenPaused = false;
    
    inputComponent->AssignAction(GLFW_KEY_UP, this, &GravityShip::MoveUp);
    inputComponent->AssignAction(GLFW_KEY_DOWN, this, &GravityShip::MoveDown);
}

void GravityShip::MoveUp(const InputData& inputData)
{
    Move({0.0f, inputData.deltaTime * 10.0f, 0.0f});
}

void GravityShip::MoveDown(const InputData& inputData)
{
    Move({0.0f, -inputData.deltaTime * 10.0f, 0.0f});
}
