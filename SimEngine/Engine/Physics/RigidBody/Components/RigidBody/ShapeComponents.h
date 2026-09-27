#pragma once

#include "Components/MeshComponent.h"

class RigidBodyShapeComponent : public MeshComponent
{
public:
    using MeshComponent::MeshComponent;
    
    void Init() override;
    
    const glm::mat3& GetInitialInertiaTensor() const { return initialInertiaTensor; }
    float GetMass() const { return mass; }
    
protected:
    void RecalculateInertiaTensor();
    
    virtual glm::mat3 CalculateInitialInertiaTensor() const = 0;
    
    glm::mat3 initialInertiaTensor{};
    float mass{1.0f};
};

class BoxShapeComponent : public RigidBodyShapeComponent
{
public:
    BoxShapeComponent(const SceneObjectParams& params);
    
    void SetSize(const glm::vec3& newSize)
    {
        size = newSize;
        SetScale(size);
    }
    
protected:
    glm::mat3 CalculateInitialInertiaTensor() const override;
    
    glm::vec3 size{1.0f};
};

class SphereShapeComponent : public RigidBodyShapeComponent
{
public:
    SphereShapeComponent(const SceneObjectParams& params);
    
protected:
    glm::mat3 CalculateInitialInertiaTensor() const override;
    
    float radius{0.5f};
};