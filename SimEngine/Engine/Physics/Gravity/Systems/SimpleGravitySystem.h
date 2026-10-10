#pragma once

#include "Physics/PhysicsSystem.h"
#include "Physics/Gravity/Components/SimpleGravityComponent.h"

class SimpleGravitySystem : public PhysicsSystem<SimpleGravityComponent, SimpleGravitySystem>
{
public:
    using PhysicsSystem::PhysicsSystem;
    
    void PhysicsTick(float physicsDeltaTime) override;
    
    PROPERTY(BoolProperty, enableGravity)
    PROPERTY(FloatProperty, gravity, -9.81f, -20.0f, 20.0f, "%.2f m/s²")
};
