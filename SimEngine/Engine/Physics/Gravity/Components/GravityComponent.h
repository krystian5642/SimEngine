#pragma once

#include "Physics/RigidBody/Components/RigidBodyComponent.h"

class GravityComponent : public RigidBodyComponent
{
public:
    GravityComponent(const SceneObjectParams& params);
    
    void Init() override;
    void OnDestroy() override;
    
    PROPERTY(BoolProperty, canAttract, true)
    PROPERTY(BoolProperty, canBeAttracted, true)
};
