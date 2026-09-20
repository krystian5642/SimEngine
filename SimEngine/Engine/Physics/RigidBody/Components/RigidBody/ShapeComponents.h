#pragma once

#include "Components/MeshComponent.h"

class RigidBodyShapeComponent : public MeshComponent
{
public:
    using MeshComponent::MeshComponent;
    
    void Init() override;
    
    const glm::mat3& GetInertiaTensor() const { return inertiaTensor; }
    float GetMass() const { return mass; }
    
protected:
    void RecalculateInertiaTensor();
    
    virtual glm::mat3 CalculateInertiaTensor() const = 0;
    
    glm::mat3 inertiaTensor{};
    float mass{1.0f};
};

class BoxShapeComponent : public RigidBodyShapeComponent
{
public:
    BoxShapeComponent(const SceneObjectParams& params);
    
protected:
    glm::mat3 CalculateInertiaTensor() const override;
    
    glm::vec3 size{1.0f};
};

class SphereShapeComponent : public RigidBodyShapeComponent
{
public:
    SphereShapeComponent(const SceneObjectParams& params);
    
protected:
    glm::mat3 CalculateInertiaTensor() const override;
    
    float radius{0.5f};
};