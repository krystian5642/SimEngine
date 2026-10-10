#pragma once

#include "PhysicsComponent.h"
#include "Core/Properties/ClassProperty.h"

class MovementComponent : public PhysicsComponent
{
public:
    using PhysicsComponent::PhysicsComponent;

    void Init() override;
    void PhysicsTick(float physicsDeltaTime) override;

    void ApplyForce(const glm::vec3& force
        , bool velocityChange = false);
    
    float GetMass() const { return mass; }
    
    const glm::vec3& GetPosition() const { return centerOfMass; }
    void SetPosition(const glm::vec3& position);
    
    void SetLinearVelocity(const glm::vec3& newLinearVelocity) { linearVelocity = newLinearVelocity; }
    
    const glm::vec3& GetLinearVelocity() const { return linearVelocity; }
    const glm::vec3& GetCenterOfMass() const { return centerOfMass; }
    
    PROPERTY(BoolProperty, canMove, true)

protected:
    glm::vec3 linearVelocity{};
    glm::vec3 centerOfMass{};
    
    float mass{};
    
    glm::vec3 accumulatedForce{};
};
