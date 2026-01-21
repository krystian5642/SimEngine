#pragma once

#include "PhysicsSystem.h"

class GravityComponent;

class GravitySystem : public PhysicsSystem
{
public:
    GravitySystem(ObjectBase* parent, Scene* scene, const std::string& name);

    void Start() override;
    void Tick(float deltaTime) override;
    
    float gravity{100.0f};
};
