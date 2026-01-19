#include "BallLauncher.h"

#include "Managers/MaterialManager.h"
#include "Managers/MeshManager.h"
#include "Components/InputComponent.h"
#include "Components/PhysicsComponent.h"
#include "Scene/Scene.h"

#include <GLFW/glfw3.h>

#include "Components/LineComponent.h"
#include "Core/PhysicsUtils.h"

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
    inputComponent->AssignAction(GLFW_KEY_M, this, &BallLauncher::DeleteLastBall);

    inputComponent->AssignAction(GLFW_KEY_LEFT, this, &BallLauncher::RotateLeft);
    inputComponent->AssignAction(GLFW_KEY_RIGHT, this, &BallLauncher::RotateRight);
    inputComponent->AssignAction(GLFW_KEY_UP, this, &BallLauncher::MoveForward);
    inputComponent->AssignAction(GLFW_KEY_DOWN, this, &BallLauncher::MoveBackward);

    physicsComponent = AddComponent<PhysicsComponent>();
    physicsComponent->physicsData.mass = 1.0f;
    physicsComponent->physicsData.enableGravity = false;

    lineComponent = AddComponent<LineComponent>();
    lineComponent->SetThickness(5.0f);
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

    auto ball = scene->AddObject<MeshEntity>();
    
    lastBallHandle = ball->GetHandle();
    
    ball->SetMesh(MeshManager::Get().GetAssetByName("sphere"));
    ball->SetMaterial(MaterialManager::Get().GetAssetByName("turquoise"));

    auto physicsComp = ball->AddComponent<PhysicsComponent>();
    physicsComp->physicsData.velocity = startVelocity;
    physicsComp->physicsData.enableGravity = true;
    physicsComp->physicsData.applyFriction = false;

    ball->SetScale({0.4f, 0.4f, 2.0f});
    ball->Rotate(barrelComponent->GetRotation());
    ball->Move(spawn);

    // dummy recoil
    const auto recoilForce = -200.0f * startVelocity;
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
    const glm::vec3 forwardEngineForce = meshComponent->GetForwardVector() * 10.f;
    physicsComponent->ApplyForce(forwardEngineForce);
}
void BallLauncher::MoveBackward(const InputData& inputData)
{
    const glm::vec3 backwardEngineForce = -meshComponent->GetForwardVector() * 10.f;
    physicsComponent->ApplyForce(backwardEngineForce);
}
glm::vec3 BallLauncher::GetBallSpawnPosition() const
{
    return barrelComponent->GetPosition() + barrelComponent->GetForwardVector() * 3.0f;
}

glm::vec3 BallLauncher::GetBallStartVelocity() const
{
    return barrelComponent->GetForwardVector() * 40.0f;
}
    
void BallLauncher::UpdateTrajectory()
{
    ProjectileTrajectoryData trajectoryData;
    trajectoryData.startPosition = GetBallSpawnPosition();
    trajectoryData.velocity = GetBallStartVelocity();
    trajectoryData.time = 100.0f;

    std::vector<glm::vec3> trajectoryPoints;
    PhysicsUtils::PredictProjectileTrajectory(trajectoryData,trajectoryPoints);

    lineComponent->SetPoints(trajectoryPoints);
}