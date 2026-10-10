
#include "MovementComponent.h"
#include "Scene/Objects/Entities/Entity.h"

void MovementComponent::Init()
{
    PhysicsComponent::Init();
    
    centerOfMass = parentEntity->GetPosition();
}

void MovementComponent::PhysicsTick(float physicsDeltaTime)
{
    if (canMove)
    {
        glm::vec3 linearAcceleration = accumulatedForce / mass;
        linearVelocity += linearAcceleration * physicsDeltaTime;
    
        const glm::vec3 moveDelta = linearVelocity * physicsDeltaTime;
        parentEntity->Move(moveDelta);
    
        centerOfMass += moveDelta;
    }
    
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

void MovementComponent::SetPosition(const glm::vec3& position)
{
    centerOfMass = position;
    parentEntity->SetPosition(position);
}
