#pragma once

#include "PhysicsComponent.h"
#include "Core/Properties/ClassProperty.h"

class ShapeComponent;

class MovementComponent : public PhysicsComponent
{
public:
    using PhysicsComponent::PhysicsComponent;

    void Init() override;
    void PhysicsTick(float physicsDeltaTime) override;
    
    void DrawUI() override;

    void ApplyForce(const glm::vec3& force
        , bool velocityChange = false);
    
    float GetMass() const { return totalMass; }
    
    glm::vec3 CalculateLinearMomentum() const;
    float CalculateLinearKineticEnergy() const;
    
    const glm::vec3& GetPosition() const { return centerOfMass; }
    void SetPosition(const glm::vec3& position);
    
    void SetLinearVelocity(const glm::vec3& newLinearVelocity) { linearVelocity = newLinearVelocity; }
    
    const glm::vec3& GetLinearVelocity() const { return linearVelocity; }
    const glm::vec3& GetCenterOfMass() const { return centerOfMass; }
    
    PROPERTY(BoolProperty, canMove, true)

protected:
    virtual void Recalculate();
    void CalculateMass();
    virtual void CalculateCenterOfMass();
    
    std::vector<ShapeComponent*> shapes;
    
    glm::vec3 linearVelocity{};
    glm::vec3 centerOfMass{};
    
    glm::vec3 prevLinearVelocity{};
    
    float totalMass{};
    
    glm::vec3 accumulatedForce{};
};
