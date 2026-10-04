
#include "ShapeComponents.h"
#include "Managers/MaterialManager.h"
#include "Managers/MeshManager.h"
#include "Rendering/Core/Material.h"

ShapeComponent::ShapeComponent(const SceneObjectParams& params, const std::string& meshName,
                               const std::string& materialName)
        : MeshComponent(params)
{
    mesh = MeshManager::Get().GetAssetByName(meshName);
    material = MaterialManager::Get().GetAssetByName(materialName, true);
    material->data.color.w = 0.7f;
    
    mass.SetOnChangedEvent(this, &ShapeComponent::OnSetMass);
}

void ShapeComponent::OnSetMass(float newMass)
{
    propertyChangedEvent.Invoke();
}

BoxComponent::BoxComponent(const SceneObjectParams& params)
    : ShapeComponent(params, "box", "emerald")
{
    size.SetOnChangedEvent(this, &BoxComponent::OnSetSize);
}

glm::mat3 BoxComponent::CalculateLocalInertiaTensor() const
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

void BoxComponent::OnSetSize(const glm::vec3& newSize)
{
    propertyChangedEvent.Invoke();
}

SphereComponent::SphereComponent(const SceneObjectParams& params)
    : ShapeComponent(params, "sphere", "emerald")
{
    radius.SetOnChangedEvent(this, &SphereComponent::OnSetRadius);
}

glm::mat3 SphereComponent::CalculateLocalInertiaTensor() const
{
    constexpr float sphereFactor = 2.0f / 5.0f;
    const float I = sphereFactor * mass * radius * radius;
    return{I};
}

void SphereComponent::OnSetRadius(float newRadius)
{
    const float diameter = 2.0f * newRadius;
    SetScale(glm::vec3{diameter});
    
    propertyChangedEvent.Invoke();
}

CylinderComponent::CylinderComponent(const SceneObjectParams& params)
    : ShapeComponent(params, "cylinder", "emerald")
{
    radius.SetOnChangedEvent(this, &CylinderComponent::OnSetRadius);
    height.SetOnChangedEvent(this, &CylinderComponent::OnSetHeight);
    
    propertyChangedEvent.Invoke();
}

glm::mat3 CylinderComponent::CalculateLocalInertiaTensor() const
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

void CylinderComponent::OnSetRadius(float newRadius)
{
    const float diameter = 2.0f * newRadius;
    SetScale(glm::vec3{diameter, height.Get(), diameter});
    
    propertyChangedEvent.Invoke();
}

void CylinderComponent::OnSetHeight(float newHeight)
{
    SetScale(glm::vec3{radius.Get(), newHeight, radius.Get()});
    
    propertyChangedEvent.Invoke();
}

CapsuleComponent::CapsuleComponent(const SceneObjectParams& params)
    : ShapeComponent(params, "capsule", "emerald")
{
    sphereRadius.SetOnChangedEvent(this, &CapsuleComponent::OnSetSphereRadius);
    cylinderHeight.SetOnChangedEvent(this, &CapsuleComponent::OnSetCylinderHeight);
}

glm::mat3 CapsuleComponent::CalculateLocalInertiaTensor() const
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

void CapsuleComponent::OnSetSphereRadius(float newSphereRadius)
{
    const float diameter = 2.0f * newSphereRadius;
    SetScale(glm::vec3{diameter, cylinderHeight.Get(), diameter});
    
    propertyChangedEvent.Invoke();
}

void CapsuleComponent::OnSetCylinderHeight(float newCylinderHeight)
{
    const float diameter = 2.0f * sphereRadius;
    SetScale(glm::vec3{diameter, newCylinderHeight, diameter});
    
    propertyChangedEvent.Invoke();
}
