#pragma once

#include "Physics/RigidBody/Components/CompoundRigidBodyComponent.h"

class GravityComponent : public CompoundRigidBodyComponent
{
public:
    GravityComponent(const SceneObjectParams& params);
    
    void Init() override;
    void OnDestroy() override;
    
    PROPERTY(BoolProperty, canAttract, true)
    PROPERTY(BoolProperty, canBeAttracted, true)
};
