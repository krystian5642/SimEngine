#pragma once

#include "Physics/PhysicsSystem.h"

class GravityComponent;

class GravitySystem : public PhysicsSystem
{
public:
    GravitySystem(ObjectBase* parent, Scene* scene, const std::string& name)
        : PhysicsSystem(parent, scene, name) {}
    
    void Tick(float deltaTime) override;
    
    float gravity{36.0f};
};
