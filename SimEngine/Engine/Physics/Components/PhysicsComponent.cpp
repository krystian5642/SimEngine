#include "PhysicsComponent.h"

#include "Scene/Objects/Entities/Entity.h"

void PhysicsComponent::Init()
{
    Component::Init();
    
    parentEntity = dynamic_cast<Entity*>(parent);
    
    /*scene->GetObjectsByClass<PhysicsSystem>(scenePhysicsSystems);
    for (const auto physicsSystem : scenePhysicsSystems)
    {
        physicsSystem->RegisterPhysicsComponent(this);
    }*/
}

void PhysicsComponent::OnDestroy()
{
    Component::OnDestroy();
    
    /*for (const auto physicsSystem : scenePhysicsSystems)
    {
        physicsSystem->UnregisterPhysicsComponent(this);
    }*/
}
