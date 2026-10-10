
#include "RigidBodyComponent.h"

#include "Physics/MovementComponent.h"
#include "Physics/RigidBody/Components/ShapeComponents.h"
#include "Scene/Objects/Entities/Entity.h"

void RigidBodyComponent::PhysicsTick(float physicsDeltaTime)
{
    MovementComponent::PhysicsTick(physicsDeltaTime);
    
    if (canRotate)
    {
        const glm::mat3 rotationMatrix = glm::mat3(parentEntity->GetRotationMatrix());
        const glm::mat3 transRotationMatrix = glm::transpose(rotationMatrix);
    
        inertiaTensor = rotationMatrix * localInertiaTensor * transRotationMatrix;
        invInertiaTensor = glm::inverse(inertiaTensor);
    
        const glm::vec3 angularAcceleration = invInertiaTensor * (accumulatedTorque
                - glm::cross(angularVelocity, inertiaTensor * angularVelocity));
        
        angularVelocity += angularAcceleration * physicsDeltaTime;
    
        const float rotationDelta = glm::length(angularVelocity) * physicsDeltaTime;
        parentEntity->Rotate(rotationDelta, angularVelocity);
    }
    
    accumulatedTorque = {};
}

void RigidBodyComponent::ApplyForceAtLocation(const glm::vec3& force, const glm::vec3& location, bool velocityChange)
{
    const glm::vec3 torque = glm::cross(location - centerOfMass, force);
    if (velocityChange)
    {
        angularVelocity += invInertiaTensor * torque;
    }
    else
    {
        accumulatedTorque += torque;
    }
    
    if (forceAtLocationAffectsLinearMotion)
    {
        ApplyForce(force, velocityChange);
    }
}

void RigidBodyComponent::UpdateShapeProperties(const ShapeComponent* shape)
{
    localInertiaTensor = shape->CalculateLocalInertiaTensor();
    mass = shape->mass;

    inertiaTensor = localInertiaTensor;
    invInertiaTensor = glm::inverse(localInertiaTensor);
}
