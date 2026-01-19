#pragma once

#include "Components/PhysicsComponent.h"
#include "Scene/Objects/Core/SceneObject.h"

class PhysicsSystem : public SceneObject
{
public:
    PhysicsSystem(ObjectBase* parent, Scene* scene, const std::string& name);
    
    void Tick(float deltaTime) override;
    
    void RegisterPhysicsComponent(PhysicsComponent* component);
    void UnregisterPhysicsComponent(PhysicsComponent* component);
    
private:
    std::vector<PhysicsComponent*> physicsComponents;
};


