#include "BallLauncher.h"

#include "GravityComponent.h"
#include "Managers/MaterialManager.h"
#include "Managers/MeshManager.h"
#include "Components/InputComponent.h"
#include "Components/PhysicsComponent.h"
#include "Scene/Scene.h"

#include <GLFW/glfw3.h>

BallLauncher::BallLauncher(ObjectBase* parent, Scene* scene, const std::string& name)
    : MeshEntity(parent, scene, name)
{
    SetMesh(MeshManager::Get().GetAssetByName("cube"));
    SetMaterial(MaterialManager::Get().GetAssetByName("bronze"));
    
    SetScale({1.5f, 1.5f, 2.5f});
    
    barrelComponent = AddComponent<MeshComponent>();
    barrelComponent->mesh = MeshManager::Get().GetAssetByName("cube");
    barrelComponent->material = MaterialManager::Get().GetAssetByName("bronze");
    barrelComponent->SetScale({0.5f, 0.5f, 4.5f});
    barrelComponent->Rotate({45.0f, 0.0f, 0.0f});
    
    inputComponent = AddComponent<InputComponent>();
    inputComponent->AssignAction(GLFW_KEY_E, this, &BallLauncher::RotateBarrelRight);
    inputComponent->AssignAction(GLFW_KEY_Q, this, &BallLauncher::RotateBarrelLeft);
    inputComponent->AssignAction(GLFW_KEY_SPACE, this, &BallLauncher::Fire);
    
    inputComponent->AssignAction(GLFW_KEY_LEFT, this, &BallLauncher::RotateLeft);
    inputComponent->AssignAction(GLFW_KEY_RIGHT, this, &BallLauncher::RotateRight);
    inputComponent->AssignAction(GLFW_KEY_UP, this, &BallLauncher::MoveForward);
    inputComponent->AssignAction(GLFW_KEY_DOWN, this, &BallLauncher::MoveBackward);
    
    physicsComponent = AddComponent<PhysicsComponent>();
    physicsComponent->physicsData.mass = 1.0f;
    physicsComponent->physicsData.enableGravity = false;
}

void BallLauncher::RotateBarrelLeft(const InputData& inputData)
{
    barrelComponent->Rotate({inputData.deltaTime * rotationSpeed, 0.0f, 0.0f});
}

void BallLauncher::RotateBarrelRight(const InputData& inputData)
{
    barrelComponent->Rotate({-inputData.deltaTime * rotationSpeed, 0.0f, 0.0f});
}

void BallLauncher::Fire(const InputData& inputData)
{
    const double currentTime = glfwGetTime();
    if (currentTime - lastFireTime < fireCooldown) return;
    lastFireTime = currentTime;
    
    auto barrelForward = barrelComponent->GetForwardVector();
    auto spawn = barrelForward * 3.0f + GetPosition();
    
    auto ball = AddChild<MeshEntity>();
    ball->SetMesh(MeshManager::Get().GetAssetByName("sphere"));
    ball->SetMaterial(MaterialManager::Get().GetAssetByName("turquoise"));
    
    auto grav = ball->AddComponent<GravityComponent>();
    grav->gravityData.mass = 100.0f;
    auto physicsComp = ball->AddComponent<PhysicsComponent>();
    physicsComp->physicsData.velocity = barrelForward * 300.0f;
    physicsComp->physicsData.enableGravity = true;
    
    ball->Init();
    ball->Start();
    
    ball->SetScale({0.4f, 0.4f, 12.0f});
    ball->Rotate(barrelComponent->GetRotation());
    ball->Move(spawn);
}

void BallLauncher::RotateLeft(const InputData& inputData)
{
    Rotate({0.0f, inputData.deltaTime * rotationSpeed, 0.0f});
}

void BallLauncher::RotateRight(const InputData& inputData)
{
    Rotate({0.0f, -inputData.deltaTime * rotationSpeed, 0.0f});   
}

void BallLauncher::MoveForward(const InputData& inputData)
{
    const glm::vec3 forwardEngineForce = meshComponent->GetForwardVector() * 10.f;
    physicsComponent->ApplyForce(forwardEngineForce);
}

void BallLauncher::MoveBackward(const InputData& inputData)
{
    const glm::vec3 backwardEngineForce = -meshComponent->GetForwardVector() * 10.f;
    physicsComponent->ApplyForce(backwardEngineForce);
}
