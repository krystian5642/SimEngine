#include "RotatingComponent.h"

#include "Scene/Objects/Entities/Entity.h"

RotatingComponent::RotatingComponent(ObjectBase* parent, Scene* scene, const std::string& name)
    : Component(parent, scene, name)
{
    parentEntity = dynamic_cast<Entity*>(parent);
}

void RotatingComponent::Tick(float deltaTime)
{
    Component::Tick(deltaTime);
    
    ApplyLocalAngularVelocity(deltaTime);
    ApplyOrbitAngularVelocity(deltaTime);
}

void RotatingComponent::SetPosition(const glm::vec3& newPosition)
{
    parentEntity->SetPosition(newPosition);
}

const glm::vec3& RotatingComponent::GetPosition() const
{
    return parentEntity->GetPosition();
}

void RotatingComponent::Rotate(const glm::quat& rotationDelta)
{
    parentEntity->Rotate(rotationDelta);
}

void RotatingComponent::ApplyLocalAngularVelocity(float deltaTime)
{
    if (parentEntity->GetUseQuaternionsForRotation())
    {
        const auto angularVelocityRad = glm::radians(rotatingData.localAngularVelocity);
        auto angularSpeed = glm::length(angularVelocityRad);
        if (angularSpeed > 1e-6f)
        {
            const auto deltaAngle = angularSpeed * deltaTime;
            const auto axis = angularVelocityRad / angularSpeed;
            
            const auto quatRotation = glm::angleAxis(deltaAngle, axis);
            Rotate(quatRotation);
        }
    }
    else
    {
        std::cout << "RotatingComponent::Tick: Rotation is not using quaternions!" << std::endl;
    }
}

void RotatingComponent::ApplyOrbitAngularVelocity(float deltaTime)
{
    const auto angularVelocityRad = glm::radians(rotatingData.orbitAngularVelocity);
    auto angularSpeed = glm::length(angularVelocityRad);
    if (angularSpeed > 1e-6f)
    {
        const auto deltaAngle = angularSpeed * deltaTime;
        const auto axis = angularVelocityRad / angularSpeed;
        
        const auto quatRotation = glm::angleAxis(deltaAngle, axis);
        
        const auto relativePosition = quatRotation * (GetPosition() - rotatingData.orbitCenter);
        SetPosition(rotatingData.orbitCenter + relativePosition);
    }
}
