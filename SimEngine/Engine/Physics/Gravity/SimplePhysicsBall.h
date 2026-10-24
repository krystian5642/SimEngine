#pragma once

#include "Scene/Objects/Entities/Entity.h"

class SimpleGravityComponent;
class SphereComponent;

class SimplePhysicsBall : public Entity
{
public:
    SimplePhysicsBall(const SceneObjectParams& params);
    
    PROPERTY(FunctionProperty, applyForce)
    PROPERTY(Vec3Property, forceToApply, {-4.0f, 0.0f, -2.0f})
    PROPERTY(BoolProperty, velocityChangeOnApplyForce, true)
    
    SphereComponent* sphere;
    SimpleGravityComponent* physicsBody;
};
