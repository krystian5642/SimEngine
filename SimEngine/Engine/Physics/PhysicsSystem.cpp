#include "PhysicsSystem.h"
#include "PhysicsComponent.h"

void PhysicsSystem::RegisterPhysicsComponent(PhysicsComponent* physicsComponent)
{
    physicsComponents.push_back(physicsComponent);
}

void PhysicsSystem::UnregisterPhysicsComponent(PhysicsComponent* physicsComponent)
{
    std::erase(physicsComponents, physicsComponent);
}
