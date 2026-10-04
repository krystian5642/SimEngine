
#include "MovementComponent.h"
#include "Scene/Objects/Entities/Entity.h"

void MovementComponent::PhysicsTick(float physicsDeltaTime)
{
    glm::vec3 linearAcceleration = accumulatedForce / mass;
    if (enableGravity)
    {
        linearAcceleration += glm::vec3{0.0f, gravity, 0.0f};
    }
    
    linearVelocity += linearAcceleration * physicsDeltaTime;
    
    const glm::vec3 moveDelta = linearVelocity * physicsDeltaTime;
    parentEntity->Move(moveDelta);
    
    centerOfMass += moveDelta;
    
    accumulatedForce = {};
}

void MovementComponent::ApplyForce(const glm::vec3& force, bool velocityChange)
{
    if (velocityChange)
    {
        linearVelocity += force / mass;
    }
    else
    {
        accumulatedForce += force;
    }
}

void MovementComponent::SetMass(float newMass)
{
    mass = newMass;
}