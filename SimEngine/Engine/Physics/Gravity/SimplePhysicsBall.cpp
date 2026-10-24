
#include "SimplePhysicsBall.h"

#include "Components/SimpleGravityComponent.h"
#include "Physics/RigidBody/Components/ShapeComponents.h"

SimplePhysicsBall::SimplePhysicsBall(const SceneObjectParams& params)
    : Entity(params)
{
    applyForce.Set([this]()
    {
        physicsBody->ApplyForce(forceToApply, velocityChangeOnApplyForce);
    });
    
    sphere = AddComponent<SphereComponent>();
    physicsBody = AddComponent<SimpleGravityComponent>();
    physicsBody->UpdateShapeProperties(sphere);
    physicsBody->enableGravity = true;
}
