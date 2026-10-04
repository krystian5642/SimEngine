#pragma once

#include "PhysicsComponent.h"

class MovementComponent : public PhysicsComponent
{
public:
    using PhysicsComponent::PhysicsComponent;
    
    void PhysicsTick(float physicsDeltaTime) override;

    void ApplyForce(const glm::vec3& force
        , bool velocityChange = true);
    
    void SetMass(float newMass);
    
    const glm::vec3& GetLinearVelocity() const { return linearVelocity; }
    const glm::vec3& GetCenterOfMass() const { return centerOfMass; }
    
    float gravity{-9.81f};
    bool enableGravity{false};

protected:
    glm::vec3 linearVelocity{};
    glm::vec3 centerOfMass{};
    
    float mass{};
    
    glm::vec3 accumulatedForce{};
};
