
#include "GravitySystem.h"

void GravitySystem::PhysicsTick(float physicsDeltaTime)
{
    for (size_t i = 0; i < components.size(); i++)
    {
        GravityComponent* phys1 = components[i];
        if (!phys1->canAttract)
        {
            continue;
        }
        
        for (size_t j = 0; j < components.size(); j++)
        {
            if (i == j)
            {
                continue;
            }
            
            GravityComponent* phys2 = components[j];
            if (!phys2->canBeAttracted || !phys2->canMove)
            {
                continue;
            }
            
            const glm::vec3 direction = phys1->GetPosition() - phys2->GetPosition();
            
            const float distanceSquared = glm::dot(direction, direction);
            if (distanceSquared < 0.01f) continue;
            
            const float distance = glm::sqrt(distanceSquared);
            const glm::vec3 directionNormalized = direction / distance;
            
            const float forceMagnitude = gravity * phys1->GetMass() * phys2->GetMass() / distanceSquared;
            
            const glm::vec3 force = forceMagnitude * directionNormalized;
            
            phys2->ApplyForce(force, false);
        }
    }
}
