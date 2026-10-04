#include "PhysicsComponent.h"

#include "Scene/Objects/Entities/Entity.h"

void PhysicsComponent::Init()
{
    Component::Init();
    
    parentEntity = dynamic_cast<Entity*>(parent);
}