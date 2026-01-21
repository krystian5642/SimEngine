#pragma once

#include "PhysicsSystem.h"

class SimpleGravitySystem : public PhysicsSystem
{
public:
    SimpleGravitySystem(ObjectBase* parent, Scene* scene, const std::string& name);
    
    void Tick(float deltaTime) override;
    
    float gravity{9.81f};
};
