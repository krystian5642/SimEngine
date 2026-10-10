#pragma once

#include "Physics/MovementComponent.h"

class ShapeComponent;

class RigidBodyComponent : public MovementComponent
{
public:
    using MovementComponent::MovementComponent;
    
    void PhysicsTick(float physicsDeltaTime) override;
    
    void ApplyForceAtLocation(const glm::vec3& force
        , const glm::vec3& location
        , bool velocityChange = false);

    void UpdateShapeProperties(const ShapeComponent* shape);
    
    const glm::vec3& GetAngularVelocity() const { return angularVelocity; }
    const glm::mat3& GetLocalInertiaTensor() const { return localInertiaTensor; }
    const glm::mat3& GetInertiaTensor() const { return inertiaTensor; }
    const glm::mat3& GetInverseInertiaTensor() const { return invInertiaTensor; }
    
    glm::vec3 CalculateAngularMomentum() const { return inertiaTensor * angularVelocity; }
    
    void SetAngularVelocity(const glm::vec3& newAngularVelocity) { angularVelocity = newAngularVelocity; }
    
    PROPERTY(BoolProperty, canRotate, true)
    
    bool forceAtLocationAffectsLinearMotion{true};
    
protected:
    glm::vec3 angularVelocity{};
    
    glm::mat3 localInertiaTensor{};
    glm::mat3 inertiaTensor{};
    glm::mat3 invInertiaTensor{};
    
    glm::vec3 accumulatedTorque{};
};
