#pragma once

#include "Physics/MovementComponent.h"

class ShapeComponent;

class CompoundRigidBodyComponent : public MovementComponent
{
public:
    using MovementComponent::MovementComponent;
    
    void Start() override;
    void PhysicsTick(float physicsDeltaTime) override;
    
    void DrawUI() override;
    
    void ApplyForceAtLocation(const glm::vec3& force
        , const glm::vec3& location
        , bool velocityChange = false);
    
    const glm::vec3& GetAngularVelocity() const { return angularVelocity; }
    const glm::mat3& GetLocalInertiaTensor() const { return totaLocalInertiaTensor; }
    const glm::mat3& GetInertiaTensor() const { return totalInertiaTensor; }
    const glm::mat3& GetInverseInertiaTensor() const { return totalInvertInertiaTensor; }
    
    glm::vec3 CalculateAngularMomentum() const { return totalInertiaTensor * angularVelocity; }
    float CalculateRotationalKineticEnergy() const;
    
    void SetAngularVelocity(const glm::vec3& newAngularVelocity);
    
    PROPERTY(BoolProperty, canRotate, true)
    
    bool forceAtLocationAffectsLinearMotion{true};
    
protected:
    void Recalculate() override;
    
    void CalculateCenterOfMass() override;
    void CalculateLocalTotalInertiaTensor();
    void CalculateTotalInertiaTensor();
    
    glm::vec3 angularVelocity{};
    
    glm::mat3 totaLocalInertiaTensor{};
    glm::mat3 totalInertiaTensor{};
    glm::mat3 totalInvertInertiaTensor{};
    
    glm::vec3 accumulatedTorque{};
};
