
#include "ShapeComponents.h"
#include "Managers/MaterialManager.h"
#include "Managers/MeshManager.h"

ShapeComponent::ShapeComponent(const SceneObjectParams& params, const std::string& meshName,
    const std::string& materialName)
        : MeshComponent(params)
{
    mesh = MeshManager::Get().GetAssetByName(meshName);
    material = MaterialManager::Get().GetAssetByName(materialName);
}

void ShapeComponent::Init()
{
    MeshComponent::Init();
    
    RecalculateInertiaTensor();
}

void ShapeComponent::RecalculateInertiaTensor()
{
    initialInertiaTensor = CalculateInitialInertiaTensor();
}

BoxComponent::BoxComponent(const SceneObjectParams& params)
    : ShapeComponent(params, "box", "emerald")
{
}

glm::mat3 BoxComponent::CalculateInitialInertiaTensor() const
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

SphereComponent::SphereComponent(const SceneObjectParams& params)
    : ShapeComponent(params, "sphere", "emerald")
{
}

glm::mat3 SphereComponent::CalculateInitialInertiaTensor() const
{
    constexpr float sphereFactor = 2.0f / 5.0f;
    const float I = sphereFactor * mass * radius * radius;
    return{I};
}

CylinderComponent::CylinderComponent(const SceneObjectParams& params)
    : ShapeComponent(params, "cylinder", "emerald")
{
}

glm::mat3 CylinderComponent::CalculateInitialInertiaTensor() const
{
    const float a = mass * radius * radius;
    const float b = mass * height * height;
    
    constexpr float cylinderFactor1 = 1.0f / 4.0f;
    constexpr float cylinderFactor2 = 1.0f / 12.0f;
    constexpr float cylinderFactor3 = 1.0f / 2.0f;
    
    glm::mat3 tensor{0.0f};
    tensor[0][0] = cylinderFactor1 * a + cylinderFactor2 * b;
    tensor[1][1] = cylinderFactor3 * a;
    tensor[2][2] = tensor[0][0];
    return tensor;
}

CapsuleComponent::CapsuleComponent(const SceneObjectParams& params)
    : ShapeComponent(params, "capsule", "emerald")
{
}

glm::mat3 CapsuleComponent::CalculateInitialInertiaTensor() const
{
    /// poczli to pozniej, to jest źle!!!!!!!!!!!!!!
    const float a = mass * sphereRadius * sphereRadius;
    const float b = mass * sphereRadius * sphereRadius;
    
    constexpr float cylinderFactor1 = 1.0f / 4.0f;
    constexpr float cylinderFactor2 = 1.0f / 12.0f;
    constexpr float cylinderFactor3 = 1.0f / 2.0f;
    
    glm::mat3 tensor{0.0f};
    tensor[0][0] = cylinderFactor1 * a + cylinderFactor2 * b;
    tensor[1][1] = cylinderFactor3 * a;
    tensor[2][2] = tensor[0][0];
    return tensor;
}
