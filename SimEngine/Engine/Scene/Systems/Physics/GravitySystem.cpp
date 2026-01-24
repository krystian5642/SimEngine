#include "GravitySystem.h"
#include "Components/PhysicsComponent.h"

GravitySystem::GravitySystem(ObjectBase* parent, Scene* scene, const std::string& name)
    : PhysicsSystem(parent, scene, name)
{
}

void GravitySystem::Start()
{
    PhysicsSystem::Start();
    
    for (const auto& physicsComponent : physicsComponents)
    {
        physicsComponent->physicsData.linearDamping = 1.0f;
        physicsComponent->physicsData.angularDamping = 1.0f;
    }
}

void GravitySystem::Tick(float deltaTime)
{
    for (size_t i = 0; i < physicsComponents.size(); i++)
    {
        const auto phys1 = physicsComponents[i];
        for (size_t j = i + 1; j < physicsComponents.size(); j++)
        {
            const auto phys2 = physicsComponents[j];
            
            const auto direction = phys2->GetPosition() - phys1->GetPosition();
            
            const auto distanceSquared = glm::dot(direction, direction);
            if (distanceSquared < 0.01f) continue;
            
            const auto distance = sqrt(distanceSquared);
            const auto directionNormalized = direction / distance;
            
            const auto forceMagnitude = gravity * phys1->physicsData.mass * phys2->physicsData.mass / distanceSquared;
            
            const auto force = forceMagnitude * directionNormalized;
            
            phys1->ApplyForce(force);
            phys2->ApplyForce(-force);
        }
    }
}
