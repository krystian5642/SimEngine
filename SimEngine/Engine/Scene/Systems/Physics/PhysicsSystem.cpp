#include "PhysicsSystem.h"

PhysicsSystem::PhysicsSystem(ObjectBase* parent, Scene* scene, const std::string& name)
    : SceneObject(parent, scene, name)
{
}

void PhysicsSystem::RegisterPhysicsComponent(PhysicsComponent* physicsComponent)
{
    physicsComponents.push_back(physicsComponent);
}

void PhysicsSystem::UnregisterPhysicsComponent(PhysicsComponent* physicsComponent)
{
    std::erase(physicsComponents, physicsComponent);
}
