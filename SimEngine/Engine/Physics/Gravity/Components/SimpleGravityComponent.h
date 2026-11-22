#pragma once

#include "Physics/RigidBody/Components/CompoundRigidBodyComponent.h"

class SimpleGravityComponent : public CompoundRigidBodyComponent
{
public:
    SimpleGravityComponent(const SceneObjectParams& params);
    
    void Init() override;
    void OnDestroy() override;
    
    PROPERTY(BoolProperty, enableGravity, false)
};
