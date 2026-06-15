#pragma once

#include "Physics/PhysicsSystem.h"

class SimpleGravitySystem : public PhysicsSystem
{
public:
    SimpleGravitySystem(ObjectBase* parent, Scene* scene, const std::string& name) 
        : PhysicsSystem(parent, scene, name) {}
    
    void Tick(float deltaTime) override;
    
    float gravity{9.81f};
    float groundLevel{-1.0f};
};
