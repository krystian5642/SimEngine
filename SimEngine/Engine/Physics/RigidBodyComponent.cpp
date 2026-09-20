#include "RigidBodyComponent.h"

#include "Scene/Objects/Entities/Entity.h"

RigidBodyComponent::RigidBodyComponent(ObjectBase* parent, Scene* scene, const std::string& name)
    : MeshComponent(parent, scene, name)
{
}

void RigidBodyComponent::Init()
{
    MeshComponent::Init();
    
    parentEntity = dynamic_cast<Entity*>(parent);
    centerOfMass = parentEntity->GetPosition();
    
    RecalculateInertiaTensor();
}

void RigidBodyComponent::Tick(float deltaTime)
{
    MeshComponent::Tick(deltaTime);
    
    if (gravityData.enabled)
    {
        accumulatedForce += glm::vec3(0.0f, gravityData.gravity, 0.0f) * mass;
    }
    
    const glm::mat3 invInertiaTensor = glm::inverse(inertiaTensor);
    
    const glm::vec3 linearAcceleration = accumulatedForce / mass;
    velocity.linearVelocity += linearAcceleration * deltaTime;
    
    const glm::vec3 moveDelta = velocity.linearVelocity * deltaTime;
    MoveParent(moveDelta);
    centerOfMass += moveDelta;
    
    const glm::vec3 angularAcceleration = invInertiaTensor * (accumulatedTorque 
    - glm::cross(velocity.angularVelocity, inertiaTensor * velocity.angularVelocity));
    velocity.angularVelocity += angularAcceleration * deltaTime;
    
    const float rotationDelta = glm::length(velocity.angularVelocity) * deltaTime;
    RotateParent(rotationDelta, velocity.angularVelocity);
    
    accumulatedForce = {};
    accumulatedTorque = {};
}

void RigidBodyComponent::SetMass(float newMass)
{
    mass = newMass;
    RecalculateInertiaTensor();
}

void RigidBodyComponent::ApplyForce(const glm::vec3& force)
{
    accumulatedForce += force;
}

void RigidBodyComponent::ApplyTorque(const glm::vec3& force, const glm::vec3& location)
{
    accumulatedTorque += glm::cross(force, location - centerOfMass);
    ApplyForce(force);
}

void RigidBodyComponent::MoveParent(const glm::vec3& moveDelta)
{
    parentEntity->Move(moveDelta);
}

void RigidBodyComponent::RotateParent(float rotationDelta, const glm::vec3& axis)
{
    parentEntity->Rotate(rotationDelta, axis);
}

void RigidBodyComponent::RecalculateInertiaTensor()
{
    inertiaTensor = CalculateInertiaTensor();
}
