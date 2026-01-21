#include "SimpleGravitySystem.h"

#include "Components/PhysicsComponent.h"

SimpleGravitySystem::SimpleGravitySystem(ObjectBase* parent, Scene* scene, const std::string& name)
    : PhysicsSystem(parent, scene, name)
{
}

void SimpleGravitySystem::Tick(float deltaTime)
{
    PhysicsSystem::Tick(deltaTime);
    
    for (const auto& physicsComponent : physicsComponents)
    {
        if (physicsComponent->physicsData.enableSimpleGravity)
        {
            physicsComponent->ApplyForce({0.0f, physicsComponent->physicsData.mass * gravity, 0.0f});
        }
    }
}
