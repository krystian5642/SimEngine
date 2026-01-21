#pragma once

#include "Scene/Objects/Core/SceneObject.h"

class PhysicsComponent;

class PhysicsSystem : public SceneObject
{
public:
    PhysicsSystem(ObjectBase* parent, Scene* scene, const std::string& name);
    virtual ~PhysicsSystem() = 0 {}
    
    void RegisterPhysicsComponent(PhysicsComponent* physicsComponent);
    void UnregisterPhysicsComponent(PhysicsComponent* physicsComponent);
    
protected:
    std::vector<PhysicsComponent*> physicsComponents;
};
