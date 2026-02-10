#include "ProjectileLauncher.h"

#include "Managers/MaterialManager.h"
#include "Managers/MeshManager.h"
#include "Components/InputComponent.h"
#include "Components/PhysicsComponent.h"
#include "Scene/Scene.h"

#include <GLFW/glfw3.h>

#include "Projectile.h"
#include "Components/LineComponent.h"
#include "Core/PhysicsUtils.h"

ProjectileLauncher::ProjectileLauncher(ObjectBase* parent, Scene* scene, const std::string& name)
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
    inputComponent->AssignAction(GLFW_KEY_C, this, &ProjectileLauncher::RotateBarrelRight);
    inputComponent->AssignAction(GLFW_KEY_Z, this, &ProjectileLauncher::RotateBarrelLeft);
    inputComponent->AssignAction(GLFW_KEY_SPACE, this, &ProjectileLauncher::Fire);
    inputComponent->AssignAction(GLFW_KEY_M, this, &ProjectileLauncher::DeleteLastBall);

    inputComponent->AssignAction(GLFW_KEY_LEFT, this, &ProjectileLauncher::RotateLeft);
    inputComponent->AssignAction(GLFW_KEY_RIGHT, this, &ProjectileLauncher::RotateRight);
    inputComponent->AssignAction(GLFW_KEY_UP, this, &ProjectileLauncher::MoveForward);
    inputComponent->AssignAction(GLFW_KEY_DOWN, this, &ProjectileLauncher::MoveBackward);

    physicsComponent = AddComponent<PhysicsComponent>();
    physicsComponent->physicsData.linearDamping = 0.4f;

    lineComponent = AddComponent<LineComponent>();
    lineComponent->SetThickness(5.0f);
}

void ProjectileLauncher::Tick(float deltaTime)
{
    MeshEntity::Tick(deltaTime);

    UpdateTrajectory();
}

void ProjectileLauncher::RotateBarrelLeft(const InputData& inputData)
{
    barrelComponent->Rotate({inputData.deltaTime * rotationSpeed, 0.0f, 0.0f});
}

void ProjectileLauncher::RotateBarrelRight(const InputData& inputData)
{
    barrelComponent->Rotate({-inputData.deltaTime * rotationSpeed, 0.0f, 0.0f});
}

void ProjectileLauncher::Fire(const InputData& inputData)
{
    const double currentTime = glfwGetTime();
    if (currentTime - lastFireTime < fireCooldown) return;
    lastFireTime = currentTime;

    const auto startVelocity = GetBallStartVelocity();
    const auto spawn = GetBallSpawnPosition();

    auto projectile = scene->AddObject<Projectile>();
    
    lastBallHandle = projectile->GetHandle();
    
    auto physicsComp = projectile->GetPhysicsComponent();
    physicsComp->physicsData.linearVelocity = startVelocity;
    physicsComp->physicsData.rotateWithCenterOfMass = false;
    physicsComp->physicsData.linearDamping = projectileLinearDamping;
    projectile->Rotate(barrelComponent->GetRotation());
    projectile->Move(spawn);

    // dummy recoil
    const auto recoilForce = -2.0f * startVelocity;
    physicsComponent->ApplyForce({recoilForce.x, 0.0f, recoilForce.z});
}

void ProjectileLauncher::DeleteLastBall(const InputData& inputData)
{
    auto las = lastBallHandle.Resolve();
    if (las == nullptr)
    {
        return;
    }
    
    las->Destroy();
}

void ProjectileLauncher::RotateLeft(const InputData& inputData)
{
    Rotate({0.0f, inputData.deltaTime * rotationSpeed, 0.0f});
}

void ProjectileLauncher::RotateRight(const InputData& inputData)
{
    Rotate({0.0f, -inputData.deltaTime * rotationSpeed, 0.0f});
}

void ProjectileLauncher::MoveForward(const InputData& inputData)
{
    const glm::vec3 forwardEngineForce = meshComponent->GetForwardVector();
    physicsComponent->ApplyForce(forwardEngineForce);
}

void ProjectileLauncher::MoveBackward(const InputData& inputData)
{
    const glm::vec3 backwardEngineForce = -meshComponent->GetForwardVector();
    physicsComponent->ApplyForce(backwardEngineForce);
}

glm::vec3 ProjectileLauncher::GetBallSpawnPosition() const
{
    return barrelComponent->GetPosition() + barrelComponent->GetForwardVector() * 3.0f;
}

glm::vec3 ProjectileLauncher::GetBallStartVelocity() const
{
    return barrelComponent->GetForwardVector() * 100.0f;
}
    
void ProjectileLauncher::UpdateTrajectory()
{
    ProjectileTrajectoryData trajectoryData;
    trajectoryData.startPosition = GetBallSpawnPosition();
    trajectoryData.velocity = GetBallStartVelocity();
    trajectoryData.time = 100.0f;
    trajectoryData.timeStep = 1.0f / 60.0f;
    trajectoryData.linearDamping = projectileLinearDamping;

    std::vector<glm::vec3> trajectoryPoints;
    PhysicsUtils::PredictProjectileTrajectory(trajectoryData,trajectoryPoints);

    lineComponent->SetPoints(trajectoryPoints);
}