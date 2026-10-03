#include "RigidBody.h"

#include "Components/VectorVisualizerComponent.h"
#include "Physics/RigidBody/Components/RigidBody/ShapeComponents.h"

RigidBody::RigidBody(const SceneObjectParams& params)
    : Entity(params)
{
    box = AddComponent<CapsuleComponent>("Box Shape");
    //box->SetSize({3.0f, 0.2f, 1.0f});
    
    vis = AddComponent<VectorVisualizerComponent>();
    vis->useParentLocationAsStart = true;
    
    SetRotationMode(RotationMode::Quaternion);
}

void RigidBody::Init()
{
    Entity::Init();
    
    initialInertiaTensor = box->GetInitialInertiaTensor();
    mass = box->GetMass();
    
    invMass = 1.0f / mass;
}

void RigidBody::PhysicsTick(float physicsDeltaTime)
{
    Entity::Tick(physicsDeltaTime);
    
    const glm::mat3 rotationMatrix = glm::mat3(GetRotationMatrix());
    const glm::mat3 transRotationMatrix = glm::transpose(rotationMatrix);
    
    inertiaTensor = rotationMatrix * initialInertiaTensor * transRotationMatrix;
    invInertiaTensor = glm::inverse(inertiaTensor);
    
    const glm::vec3 linearAcceleration = accumulatedForce * invMass;
    
    velocity.linearVelocity += linearAcceleration * physicsDeltaTime;
    
    const glm::vec3 moveDelta = velocity.linearVelocity * physicsDeltaTime;
    Move(moveDelta);
    
    centerOfMass += moveDelta;
    
    const glm::vec3 angularAcceleration = invInertiaTensor * (accumulatedTorque
            - glm::cross(velocity.angularVelocity, inertiaTensor * velocity.angularVelocity));
        
    velocity.angularVelocity += angularAcceleration * physicsDeltaTime;
    
    const float rotationDelta = glm::length(velocity.angularVelocity) * physicsDeltaTime;
    Rotate(rotationDelta, velocity.angularVelocity);
    
    vis->SetDirection(inertiaTensor * velocity.angularVelocity);
    
    accumulatedForce = {};
    accumulatedTorque = {};
}

void RigidBody::ApplyForce(const glm::vec3& force, bool velocityChange)
{
    if (velocityChange)
    {
        velocity.linearVelocity += force / mass;
    }
    else
    {
        accumulatedForce += force;
    }
}

void RigidBody::ApplyTorque(const glm::vec3& force, const glm::vec3& location, bool velocityChange)
{
    const glm::vec3 torque = glm::cross(location - centerOfMass, force);
    if (velocityChange)
    {
        velocity.angularVelocity += inertiaTensor * torque;
    }
    else
    {
        accumulatedTorque += torque;
    }
    
    //ApplyForce(force, velocityChange);
}

void RigidBody::UpdateVisualizationComponents()
{
    /*if (centerOfMassVisLine->visible)
    {
        centerOfMassVisLine->GetLine()->AddPoint(centerOfMass);
    }
    
    if (centerOfMassVisMesh->visible)
    {
        centerOfMassVisMesh->SetPosition(centerOfMass);
    }
    
    if (linearVelocityVisComp->visible)
    {
        linearVelocityVisComp->SetStart(centerOfMass);
        linearVelocityVisComp->SetDirection(velocity.linearVelocity);
    }
    
    if (angularVelocityVisComp->visible)
    {
        angularVelocityVisComp->SetStart(centerOfMass);
        angularVelocityVisComp->SetDirection(velocity.angularVelocity);
    }
    
    if (momentumVisComp->visible)
    {
        momentumVisComp->SetStart(centerOfMass);
        momentumVisComp->SetDirection(velocity.linearVelocity * totalMass);
    }
    
    if (angularMomentumVisComp->visible)
    {
        angularMomentumVisComp->SetStart(centerOfMass);
        angularMomentumVisComp->SetDirection(totalInertiaTensor * velocity.angularVelocity);
    }*/
}
