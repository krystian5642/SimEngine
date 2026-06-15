#include "SimpleGravitySystem.h"

#include "Physics/PhysicsComponent.h"

void SimpleGravitySystem::Tick(float deltaTime)
{
    PhysicsSystem::Tick(deltaTime);
    
    for (auto physicsComponent : physicsComponents)
    {
        auto& physicsData = physicsComponent->physicsData;
        if (!physicsData.canFall)
        {
            continue;
        }
        
        const auto radius = physicsComponent->GetRadius();
        
        const auto minY = radius + groundLevel;
        if (physicsComponent->GetPosition().y < radius + groundLevel)
        {
            const auto vel2 = glm::dot(physicsData.linearVelocity, physicsData.linearVelocity);
            if (vel2 < 0.9f)
            {
                physicsData.linearVelocity.y = 0.0f;
            }
            else
            {
                physicsComponent->Move({0.0f, std::abs(physicsComponent->GetPosition().y - minY) + 0.01f, 0.0f});
                physicsData.linearVelocity = glm::reflect(physicsData.linearVelocity, {0.0f, 1.0f, 0.0f}) * physicsData.restitution;
                
                physicsComponent->ApplyForce(glm::vec3(0.0f, -gravity, 0.0f) * physicsData.mass);
            }
        }
        else
        {
            physicsComponent->ApplyForce(glm::vec3(0.0f, -gravity, 0.0f) * physicsData.mass);
        }
    }
}
