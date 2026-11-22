
#include "MovementComponent.h"

#include "Core/UICommon/UICommon.h"
#include "RigidBody/Components/ShapeComponents.h"
#include "Scene/Objects/Entities/Entity.h"

void MovementComponent::Init()
{
    PhysicsComponent::Init();
    
    Recalculate();
}

void MovementComponent::PhysicsTick(float physicsDeltaTime)
{
    if (canMove)
    {
        glm::vec3 linearAcceleration = accumulatedForce / totalMass;
        linearVelocity += linearAcceleration * physicsDeltaTime;
    
        const glm::vec3 moveDelta = linearVelocity * physicsDeltaTime;
        parentEntity->Move(moveDelta);
    }
    
    CalculateCenterOfMass();
    
    accumulatedForce = {};
}

void MovementComponent::DrawUI()
{
    PhysicsComponent::DrawUI();
    
    UICommon::DrawVec3ReadOnly("Center of Mass", centerOfMass);
    
    UICommon::DrawVec3ReadOnly("Linear Momentum", CalculateLinearMomentum());
    UICommon::DrawVec3ReadOnly("Linear Velocity", linearVelocity);
    
    const float linearEnergy = CalculateLinearKineticEnergy();
    UICommon::DrawFloatReadOnly("Linear Kinetic Energy", linearEnergy, "%.3f J");
}

void MovementComponent::ApplyForce(const glm::vec3& force, bool velocityChange)
{
    if (velocityChange)
    {
        linearVelocity += force / totalMass;
    }
    else
    {
        accumulatedForce += force;
    }
}

glm::vec3 MovementComponent::CalculateLinearMomentum() const
{
    return totalMass * linearVelocity;
}

float MovementComponent::CalculateLinearKineticEnergy() const
{
    return 0.5f * totalMass * glm::dot(linearVelocity, linearVelocity);
}

void MovementComponent::SetPosition(const glm::vec3& position)
{
    centerOfMass = position;
    parentEntity->SetPosition(position);
}

void MovementComponent::Recalculate()
{
    shapes.clear();
    
    parentEntity->GetComponentsByClass<ShapeComponent>(shapes);
    
    CalculateMass();
    CalculateCenterOfMass();
}

void MovementComponent::CalculateMass()
{
    totalMass = 0.0f;
    for (auto* shape : shapes)
    {
        totalMass+= shape->mass;
    }
}

void MovementComponent::CalculateCenterOfMass()
{
    centerOfMass = parentEntity->GetPosition();
}
