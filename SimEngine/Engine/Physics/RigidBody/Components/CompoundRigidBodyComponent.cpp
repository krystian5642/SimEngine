
#include "CompoundRigidBodyComponent.h"

#include "Core/UICommon/UICommon.h"
#include "Physics/MovementComponent.h"
#include "Physics/RigidBody/Components/ShapeComponents.h"
#include "Scene/Objects/Entities/Entity.h"

static glm::mat3 ParallelAxisMatrix(const glm::vec3& offset)
{
    const float xx = offset.x * offset.x;
    const float yy = offset.y * offset.y;
    const float zz = offset.z * offset.z;

    glm::mat3 result;
    result[0][0] = yy + zz;
    result[1][1] = xx + zz;
    result[2][2] = xx + yy;

    result[0][1] = result[1][0] = -offset.x * offset.y;
    result[0][2] = result[2][0] = -offset.x * offset.z;
    result[1][2] = result[2][1] = -offset.y * offset.z;
    
    return result;
}

void CompoundRigidBodyComponent::Start()
{
    MovementComponent::Start();
    
    parentEntity->SetRotationMode(RotationMode::Quaternion);
}

void CompoundRigidBodyComponent::PhysicsTick(float physicsDeltaTime)
{
    MovementComponent::PhysicsTick(physicsDeltaTime);
    
    if (canRotate)
    {
        CalculateTotalInertiaTensor();
        
        const glm::vec3 angularAcceleration = totalInvertInertiaTensor * (accumulatedTorque
                - glm::cross(angularVelocity, totalInertiaTensor * angularVelocity));
        
        angularVelocity += angularAcceleration * physicsDeltaTime;
        
        const float angularSpeed = glm::length(angularVelocity);
        if (angularSpeed > 0.0f)
        {
            const float rotationDelta = angularSpeed * physicsDeltaTime;
            const glm::quat quatRotDelta = glm::angleAxis(rotationDelta, angularVelocity / angularSpeed);
        
            for (auto* shape : shapes)
            {
                shape->SetPosition(centerOfMass + quatRotDelta * (shape->GetPosition() - centerOfMass));
            }
        
            parentEntity->Rotate(quatRotDelta);
        }
    }
    
    accumulatedTorque = {};
}

void CompoundRigidBodyComponent::DrawUI()
{
    MovementComponent::DrawUI();
    
    UICommon::DrawVec3ReadOnly("Angular Velocity", angularVelocity);
    UICommon::DrawMat3ReadOnly("Local Total Inertia", totaLocalInertiaTensor);
    UICommon::DrawMat3ReadOnly("Inertia Tensor", totalInertiaTensor);
    UICommon::DrawVec3ReadOnly("Angular Momentum", CalculateAngularMomentum());
    
    const float rotationalEnergy = CalculateRotationalKineticEnergy();
    UICommon::DrawFloatReadOnly("Rotational Kinetic Energy", rotationalEnergy, "%.3f J");
    UICommon::DrawFloatReadOnly("Total Kinetic Energy", CalculateLinearKineticEnergy() + rotationalEnergy, "%.3f J");
}

void CompoundRigidBodyComponent::ApplyForceAtLocation(const glm::vec3& force, const glm::vec3& location, bool velocityChange)
{
    const glm::vec3 torque = glm::cross(location - centerOfMass, force);
    if (velocityChange)
    {
        angularVelocity += totalInvertInertiaTensor * torque;
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

float CompoundRigidBodyComponent::CalculateRotationalKineticEnergy() const
{
    return 0.5f * glm::dot(angularVelocity, totalInertiaTensor * angularVelocity);
}

void CompoundRigidBodyComponent::SetAngularVelocity(const glm::vec3& newAngularVelocity)
{
    angularVelocity = newAngularVelocity;
}

void CompoundRigidBodyComponent::Recalculate()
{
    MovementComponent::Recalculate();
    
    CalculateLocalTotalInertiaTensor();
}

void CompoundRigidBodyComponent::CalculateCenterOfMass()
{
    totalMass = 0.0f;
    centerOfMass = glm::vec3(0.0f);
    
    for (auto* shape : shapes)
    {
        const float mass = shape->mass;
        centerOfMass += shape->GetPosition() * mass;
        totalMass += mass;
    }
    centerOfMass /= totalMass;
}

void CompoundRigidBodyComponent::CalculateLocalTotalInertiaTensor()
{
    totaLocalInertiaTensor = glm::mat3{0.0f};
    
    for (auto* shape : shapes)
    {
        const glm::vec3 offset = centerOfMass - shape->GetPosition();
        const glm::mat3 shapeInertiaLocal  = shape->CalculateLocalInertiaTensor();
        const glm::mat3 inertiaTensor = shapeInertiaLocal + shape->mass.Get() * ParallelAxisMatrix(offset);
        
        totaLocalInertiaTensor += inertiaTensor;
    }
    
    CalculateTotalInertiaTensor();
}

void CompoundRigidBodyComponent::CalculateTotalInertiaTensor()
{
    const glm::mat3 rotationMatrix = glm::mat3(parentEntity->GetRotationMatrix());
    const glm::mat3 transRotationMatrix = glm::transpose(rotationMatrix);
    
    totalInertiaTensor = rotationMatrix * totaLocalInertiaTensor * transRotationMatrix;
    totalInvertInertiaTensor = glm::inverse(totalInertiaTensor);
}
