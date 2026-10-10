#pragma once

#include "Physics/PhysicsSystem.h"
#include "Core/Properties/ClassProperty.h"
#include "Physics/Gravity/Components/GravityComponent.h"

class GravitySystem : public PhysicsSystem<GravityComponent, GravitySystem>
{
public:
    using PhysicsSystem::PhysicsSystem;
    
    void PhysicsTick(float physicsDeltaTime) override;
    
    PROPERTY(FloatProperty, gravity, 2.0f, -10.0f, 10.0f, "%.3f m³/(kg·s²)")
};

