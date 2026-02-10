#include "SpaceShip.h"

#include <GLFW/glfw3.h>

#include "Managers/MaterialManager.h"
#include "Managers/MeshManager.h"
#include "Components/InputComponent.h"
#include "Components/PhysicsComponent.h"

SpaceShip::SpaceShip(ObjectBase* parent, Scene* scene, const std::string& name)
    : MeshEntity(parent, scene, name)
{
    SetMesh(MeshManager::Get().GetAssetByName("spaceship"));
    SetMaterial(MaterialManager::Get().GetAssetByName("gold"));
    
    physicsComponent = AddComponent<PhysicsComponent>();
    
    physicsComponent->physicsData.mass = 0.1f;
    physicsComponent->physicsData.rotateWithCenterOfMass = false;
    physicsComponent->physicsData.linearDamping = 1.0f;
    physicsComponent->physicsData.angularDamping = 1.0f;
    physicsComponent->radiusMultiplier = 6.0f;
    physicsComponent->physicsData.canBounce = false;
    physicsComponent->physicsData.stopAtCollision = true;
    
    inputComponent = AddComponent<InputComponent>();
  
    inputComponent->AssignAction(GLFW_KEY_LEFT, this, &SpaceShip::RotateYLeft);
    inputComponent->AssignAction(GLFW_KEY_RIGHT, this, &SpaceShip::RotateYRight);
    
    inputComponent->AssignAction(GLFW_KEY_Z, this, &SpaceShip::RotateXLeft);
    inputComponent->AssignAction(GLFW_KEY_X, this, &SpaceShip::RotateXRight);
    
    inputComponent->AssignAction(GLFW_KEY_C, this, &SpaceShip::RotateZLeft);
    inputComponent->AssignAction(GLFW_KEY_V, this, &SpaceShip::RotateZRight);
    
    inputComponent->AssignAction(GLFW_KEY_UP, this, &SpaceShip::MoveForward);
    inputComponent->AssignAction(GLFW_KEY_DOWN, this, &SpaceShip::MoveBackward);
    inputComponent->AssignAction(GLFW_KEY_N, this, &SpaceShip::MoveUp);
    inputComponent->AssignAction(GLFW_KEY_M, this, &SpaceShip::MoveDown);
    inputComponent->AssignAction(GLFW_KEY_SPACE, this, &SpaceShip::Stop);
}

void SpaceShip::RotateYLeft(const InputData& inputData)
{
    Rotate({0.0f, rotationSpeed * inputData.deltaTime, 0.0f});
}

void SpaceShip::RotateYRight(const InputData& inputData)
{
    Rotate({0.0f, -rotationSpeed * inputData.deltaTime, 0.0f});
}

void SpaceShip::RotateXLeft(const InputData& inputData)
{
    Rotate({rotationSpeed * inputData.deltaTime, 0.0f, 0.0f});
}

void SpaceShip::RotateXRight(const InputData& inputData)
{
    Rotate({-rotationSpeed * inputData.deltaTime, 0.0f, 0.0f});
}

void SpaceShip::RotateZLeft(const InputData& inputData)
{
    Rotate({0.0f, 0.0f, rotationSpeed * inputData.deltaTime});
}

void SpaceShip::RotateZRight(const InputData& inputData)
{
    Rotate({0.0f, 0.0f, -rotationSpeed * inputData.deltaTime});
}

void SpaceShip::MoveForward(const InputData& inputData)
{
    const glm::vec3 impulse = -meshComponent->GetForwardVector() * engineImpulse * inputData.deltaTime;
    physicsComponent->ApplyImpulse(impulse);
}

void SpaceShip::MoveBackward(const InputData& inputData)
{
    const glm::vec3 impulse = meshComponent->GetForwardVector() * engineImpulse * inputData.deltaTime;
    physicsComponent->ApplyImpulse(impulse);
}

void SpaceShip::MoveUp(const InputData& inputData)
{
    const glm::vec3 impulse = meshComponent->GetUpVector() *engineImpulse * inputData.deltaTime;
    physicsComponent->ApplyImpulse(impulse);
}

void SpaceShip::MoveDown(const InputData& inputData)
{
    const glm::vec3 impulse = -meshComponent->GetUpVector() * engineImpulse * inputData.deltaTime;
    physicsComponent->ApplyImpulse(impulse);
}

void SpaceShip::Stop(const InputData& inputData)
{
    physicsComponent->StopImediately();
}
