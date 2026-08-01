
#include "BallLauncher.h"

#include "Managers/MaterialManager.h"
#include "Managers/MeshManager.h"
#include "Components/InputComponent.h"
#include "Physics/PhysicsComponent.h"
#include "Scene/Scene.h"

#include <GLFW/glfw3.h>

#include "Components/LineComponent.h"
#include "Physics/PhysicsUtils.h"

BallLauncher::BallLauncher(ObjectBase* parent, Scene* scene, const std::string& name)
    : MeshEntity(parent, scene, name)
{
    meshComponent->mesh = MeshManager::Get().GetAssetByName("cube");
    meshComponent->material = MaterialManager::Get().GetAssetByName("bronze");

    SetScale({1.5f, 1.5f, 2.5f});

    barrelComponent = AddComponent<MeshComponent>();
    barrelComponent->mesh = MeshManager::Get().GetAssetByName("cube");
    barrelComponent->material = MaterialManager::Get().GetAssetByName("bronze");
    barrelComponent->SetScale({0.5f, 0.5f, 4.5f});
    barrelComponent->Rotate({45.0f, 0.0f, 0.0f});

    inputComponent = AddComponent<InputComponent>();
    inputComponent->AssignAction(GLFW_KEY_C, this, &BallLauncher::RotateBarrelRight);
    inputComponent->AssignAction(GLFW_KEY_Z, this, &BallLauncher::RotateBarrelLeft);
    inputComponent->AssignAction(GLFW_KEY_SPACE, this, &BallLauncher::Fire);
    inputComponent->AssignAction(GLFW_KEY_M, this, &BallLauncher::DeleteLastBall);

    inputComponent->AssignAction(GLFW_KEY_LEFT, this, &BallLauncher::RotateLeft);
    inputComponent->AssignAction(GLFW_KEY_RIGHT, this, &BallLauncher::RotateRight);
    inputComponent->AssignAction(GLFW_KEY_UP, this, &BallLauncher::MoveForward);
    inputComponent->AssignAction(GLFW_KEY_DOWN, this, &BallLauncher::MoveBackward);

    physicsComponent = AddComponent<PhysicsComponent>();
    physicsComponent->physicsData.linearDamping = 0.4f;
    physicsComponent->physicsData.canFall = false;

    lineComponent = AddComponent<LineComponent>();
    lineComponent->GetLine()->thickness = 5.0f;
}

void BallLauncher::Tick(float deltaTime)
{
    MeshEntity::Tick(deltaTime);

    UpdateTrajectory();
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

    const auto startVelocity = GetBallStartVelocity();
    const auto spawn = GetBallSpawnPosition();

    auto projectile = scene->AddObject<MeshEntity>("Projectile");
    projectile->meshComponent->mesh = MeshManager::Get().GetAssetByName("sphere");
    projectile->meshComponent->material = MaterialManager::Get().GetAssetByName("copper");
    projectile->SetScale({0.3f, 0.3f, 0.3f});

    lastBallHandle = projectile->GetHandle();
    
    auto physicsComp = projectile->AddComponent<PhysicsComponent>();
    physicsComp->physicsData.linearVelocity = startVelocity;
    physicsComp->physicsData.restitution = 0.7f;
    physicsComp->physicsData.linearDamping = ballLinearDamping;
    projectile->Rotate(barrelComponent->GetRotation());
    projectile->Move(spawn);

    //dummy recoil
    const auto recoilForce = -2.0f * startVelocity * 20.0f;
    physicsComponent->ApplyForce({recoilForce.x, 0.0f, recoilForce.z});
}

void BallLauncher::DeleteLastBall(const InputData& inputData)
{
    auto las = lastBallHandle.Resolve();
    if (las == nullptr)
    {
        return;
    }
    
    las->Destroy();
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
    const glm::vec3 forwardEngineForce = meshComponent->GetForwardVector() * 5.0f;
    physicsComponent->ApplyForce(forwardEngineForce);
}

void BallLauncher::MoveBackward(const InputData& inputData)
{
    const glm::vec3 backwardEngineForce = -meshComponent->GetForwardVector() * 5.0f;
    physicsComponent->ApplyForce(backwardEngineForce);
}

glm::vec3 BallLauncher::GetBallSpawnPosition() const
{
    return barrelComponent->GetPosition() + barrelComponent->GetForwardVector() * 3.0f;
}

glm::vec3 BallLauncher::GetBallStartVelocity() const
{
    return barrelComponent->GetForwardVector() * 10.0f;
}
    
void BallLauncher::UpdateTrajectory()
{
    ProjectileTrajectoryData trajectoryData;
    trajectoryData.startPosition = GetBallSpawnPosition();
    trajectoryData.velocity = GetBallStartVelocity();
    trajectoryData.time = 100.0f;
    trajectoryData.timeStep = 1.0f / 60.0f;
    trajectoryData.linearDamping = ballLinearDamping;

    std::vector<glm::vec3> trajectoryPoints;
    PhysicsUtils::PredictProjectileTrajectory(trajectoryData,trajectoryPoints);

    lineComponent->GetLine()->SetPoints(trajectoryPoints);
}