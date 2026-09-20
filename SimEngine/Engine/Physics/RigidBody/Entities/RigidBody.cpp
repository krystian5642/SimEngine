#include "RigidBody.h"
#include "Physics/RigidBody/Components/RigidBody/ShapeComponents.h"
#include "Components/MeshComponent.h"
#include "Components/LineComponent.h"
#include "Components/VectorVisualizerComponent.h"

CompoundRigidBody::CompoundRigidBody(const SceneObjectParams& params)
    : Entity(params)
{
    centerOfMassVisMesh = AddComponent<MeshComponent>("Center of Mass");
    rotationAxisVisMesh = AddComponent<MeshComponent>("Rotation Axis");
    
    linearVelocityVisComp = AddComponent<VectorVisualizerComponent>("Linear Velocity");
    angularVelocityVisComp = AddComponent<VectorVisualizerComponent>("Angular Velocity");
    
    momentumVisComp = AddComponent<VectorVisualizerComponent>("Momentum");
    angularMomentumVisComp = AddComponent<VectorVisualizerComponent>("Angular Momentum");
    
    centerOfMassVisLine = AddComponent<LineComponent>("Center of Mass Trajectory");
}

void CompoundRigidBody::Init()
{
    Entity::Init();
    
    for (const auto* shape : shapes)
    {
        totalInertiaTensor += shape->GetInertiaTensor();
        totalMass += shape->GetMass();
        
        centerOfMass += shape->GetMass() * shape->GetPosition();
    }
    
    invTotalInertiaTensor = glm::inverse(totalInertiaTensor);
    invTotalMass = 1.0f / totalMass;
    
    centerOfMass *= invTotalMass;
}

void CompoundRigidBody::Tick(float deltaTime)
{
    Entity::Tick(deltaTime);
    
    const glm::vec3 linearAcceleration = accumulatedForce * invTotalMass;
    velocity.linearVelocity += linearAcceleration * deltaTime;
    
    const glm::vec3 moveDelta = velocity.linearVelocity * deltaTime;
    Move(moveDelta);
    
    centerOfMass += moveDelta;
    
    const glm::vec3 angularAcceleration = invTotalInertiaTensor * (accumulatedTorque 
        - glm::cross(velocity.angularVelocity, totalInertiaTensor * velocity.angularVelocity));
    velocity.angularVelocity += angularAcceleration * deltaTime;
    
    // const float rotationDelta = glm::length(velocity.angularVelocity) * deltaTime;
    // RotateParent(rotationDelta, velocity.angularVelocity)
    
    accumulatedForce = {};
    accumulatedTorque = {};
}

void CompoundRigidBody::ApplyForce(const glm::vec3& force, bool velocityChange)
{
    if (velocityChange)
    {
        velocity.linearVelocity += force / totalMass;
    }
    else
    {
        accumulatedForce += force;
    }
}

void CompoundRigidBody::ApplyTorque(const glm::vec3& force, const glm::vec3& location, bool velocityChange)
{
    const glm::vec3 torque = glm::cross(location - centerOfMass, force);
    if (velocityChange)
    {
        velocity.angularVelocity += totalInertiaTensor * torque;
    }
    else
    {
        accumulatedTorque += torque;
    }
}

void CompoundRigidBody::UpdateVisualizationComponents()
{
    if (centerOfMassVisLine->visible)
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
    }
}
