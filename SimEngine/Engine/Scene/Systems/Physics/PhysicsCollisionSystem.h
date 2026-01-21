#pragma once

#include "PhysicsSystem.h"

class PhysicsCollisionSystem : public PhysicsSystem
{
public:
    PhysicsCollisionSystem(ObjectBase* parent, Scene* scene, const std::string& name);
    
    void Tick(float deltaTime) override;
};


