
#include "SimpleGravitySystem.h"

void SimpleGravitySystem::PhysicsTick(float physicsDeltaTime)
{
    if (!enableGravity)
    {
        return;    
    }
    
    for (SimpleGravityComponent* component : components)
    {
        if (!component->canMove || !component->enableGravity)
        {
            continue;
        }
        
        component->ApplyForce(component->GetMass() * glm::vec3{ 0.0f, gravity.Get(), 0.0f }, false);
    }
}