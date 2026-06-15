#pragma once

#include "Physics/PhysicsSystem.h"

class PhysicsCollisionSystem : public PhysicsSystem
{
public:
    PhysicsCollisionSystem(ObjectBase* parent, Scene* scene, const std::string& name)
        : PhysicsSystem(parent, scene, name) {}
    
    void Tick(float deltaTime) override;
};


