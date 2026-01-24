#include "GravityShip.h"

#include <GLFW/glfw3.h>

#include "Components/InputComponent.h"
#include "Components/PhysicsComponent.h"
#include "Managers/MaterialManager.h"
#include "Managers/MeshManager.h"
#include "Scene/Objects/Entities/MeshEntity.h"

GravityShip::GravityShip(ObjectBase* parent, Scene* scene, const std::string& name)
    : Entity(parent, scene, name)
{
    constexpr float mass = 5.0f;
    
    left = AddChild<MeshEntity>();
    
    left->SetMesh(MeshManager::Get().GetAssetByName("sphere"));
    left->SetMaterial(MaterialManager::Get().GetAssetByName("chrome"));
    left->Move({0.0f, 0.0f, -2.0f});
    left->SetScale({0.5f, 1.5f, 0.5f});
    
    physicsComponentLeft = left->AddComponent<PhysicsComponent>();
    physicsComponentLeft->physicsData.mass = mass;
    physicsComponentLeft->physicsData.physicsLinearConstraints = {true, false, true};
    
    
    right = AddChild<MeshEntity>();
    
    right->SetMesh(MeshManager::Get().GetAssetByName("sphere"));
    right->SetMaterial(MaterialManager::Get().GetAssetByName("chrome"));
    right->Move({4.0f, 0.0f, -2.0f});
    right->SetScale({0.5f, 1.5f, 0.5f});
    
    physicsComponentRight = right->AddComponent<PhysicsComponent>();
    physicsComponentRight->physicsData.mass = mass;
    physicsComponentRight->physicsData.physicsLinearConstraints = {true, false, true};
    
    inputComponent = AddComponent<InputComponent>();
    inputComponent->tickWhenPaused = false;
    
    inputComponent->AssignAction(GLFW_KEY_UP, this, &GravityShip::MoveUp);
    inputComponent->AssignAction(GLFW_KEY_DOWN, this, &GravityShip::MoveDown);
}

void GravityShip::MoveUp(const InputData& inputData)
{
    Move({0.0f, inputData.deltaTime * 10.0f, 0.0f});
    
    /*const glm::vec3 force = {0.0f, inputData.deltaTime * 100000.0f, 0.0f};
    physicsComponentLeft->ApplyForce(force);
    physicsComponentRight->ApplyForce(force);*/
}

void GravityShip::MoveDown(const InputData& inputData)
{
    Move({0.0f, -inputData.deltaTime * 10.0f, 0.0f});
    /*const glm::vec3 force = {0.0f, -inputData.deltaTime * 100000.0f, 0.0f};
    physicsComponentLeft->ApplyForce(force);
    physicsComponentRight->ApplyForce(force);*/
}
