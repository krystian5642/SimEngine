
#include "ShapeComponents.h"
#include "Managers/MaterialManager.h"
#include "Managers/MeshManager.h"

void RigidBodyShapeComponent::Init()
{
    MeshComponent::Init();
    
    RecalculateInertiaTensor();
}

void RigidBodyShapeComponent::RecalculateInertiaTensor()
{
    inertiaTensor = CalculateInertiaTensor();
}

BoxShapeComponent::BoxShapeComponent(const SceneObjectParams& params)
    : RigidBodyShapeComponent(params)
{
    mesh = MeshManager::Get().GetAssetByName("box");
    material = MaterialManager::Get().GetAssetByName("emerald");
}

glm::mat3 BoxShapeComponent::CalculateInertiaTensor() const
{
    const float a2 = size.x * size.x;
    const float b2 = size.y * size.y;
    const float c2 = size.z * size.z;
    constexpr float boxFactor = 1.0f / 12.0f;

    glm::mat3 tensor{0.0f};
    tensor[0][0] = boxFactor * mass * (b2 + c2);
    tensor[1][1] = boxFactor * mass * (a2 + c2);
    tensor[2][2] = boxFactor * mass * (a2 + b2);
    return tensor;
}

SphereShapeComponent::SphereShapeComponent(const SceneObjectParams& params)
    : RigidBodyShapeComponent(params)
{
    mesh = MeshManager::Get().GetAssetByName("sphere");
    material = MaterialManager::Get().GetAssetByName("emerald");
}

glm::mat3 SphereShapeComponent::CalculateInertiaTensor() const
{
    constexpr float sphereFactor = 2.0f / 5.0f;
    const float I = sphereFactor * mass * radius * radius;
    return{I};
}