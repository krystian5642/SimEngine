
#include "PhysicsSpringComponent.h"
#include "Physics/MovementComponent.h"

PhysicsSpringComponent::PhysicsSpringComponent(const SceneObjectParams& params)
    : SpringComponent(params)
{
    start.readOnly = true;
    end.readOnly = true;
}

void PhysicsSpringComponent::PhysicsTick(float physicsDeltaTime)
{
    if (!startComponent && !endComponent)
    {
        return;
    }
    
    if (startComponent)
    {
        start = startComponent->GetPosition();
    }
   
    if (endComponent)
    {
        end = endComponent->GetPosition();
    }
    
    const glm::vec3 equilibriumPosition = start.Get() + glm::normalize(end.Get() - start.Get()) * baseLenght.Get();
    const glm::vec3 force = springConstant.Get() * (equilibriumPosition - end.Get());
    
    if (startComponent)
    {
        startComponent->ApplyForce(-force);
    }
    
    if (endComponent)
    {
        endComponent->ApplyForce(force);
    }
}

void PhysicsSpringComponent::AttachComponentToStart(MovementComponent* movementComponent)
{
    startComponent = movementComponent;
}

void PhysicsSpringComponent::AttachComponentToEnd(MovementComponent* movementComponent)
{
    endComponent = movementComponent;
}
