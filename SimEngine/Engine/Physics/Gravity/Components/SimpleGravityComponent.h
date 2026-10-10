#pragma once

#include "Physics/RigidBody/Components/RigidBodyComponent.h"

class SimpleGravityComponent : public RigidBodyComponent
{
public:
    SimpleGravityComponent(const SceneObjectParams& params);
    
    void Init() override;
    void OnDestroy() override;
    
    PROPERTY(BoolProperty, enableGravity, false)
};
