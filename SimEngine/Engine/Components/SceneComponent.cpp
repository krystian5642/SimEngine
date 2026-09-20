#include "SceneComponent.h"

#include "Core/MathUtils.h"

SceneComponent::SceneComponent(const SceneObjectParams& params)
    : RenderComponent(params)
{
    UpdateVectors();
}

void SceneComponent::Move(const glm::vec3& moveDelta)
{
    if (MathUtils::IsNearlyZeroVector(moveDelta))
    {
        return;
    }
    
    transform.SetPosition(transform.GetPosition() + moveDelta);
    UpdateVectors();

    for (const auto& attachedComponent : attachedComponents)
    {
        attachedComponent->Move(moveDelta);
    }
}

void SceneComponent::Rotate(const glm::vec3& rotateDelta)
{
    if (MathUtils::IsNearlyZeroVector(rotateDelta))
    {
        return;
    }
    
    transform.SetPosition(transform.GetEulerRotation() + rotateDelta);
    UpdateVectors();

    for (const auto& attachedComponent : attachedComponents)
    {
        attachedComponent->Rotate(rotateDelta);
    }
}

void SceneComponent::SetRotationMode(RotationMode newRotationMode)
{
    transform.SetRotationMode(newRotationMode);
    
    UpdateVectors();

    for (const auto& attachedComponent : attachedComponents)
    {
        attachedComponent->SetRotationMode(newRotationMode);
    }
}

void SceneComponent::SetEulerRotation(const glm::vec3& newRotation)
{
    transform.SetEulerRotation(newRotation);
    UpdateVectors();

    for (const auto& attachedComponent : attachedComponents)
    {
        attachedComponent->SetEulerRotation(newRotation);
    }
}

void SceneComponent::SetQuatRotation(const glm::quat& newQuatRotation)
{
    transform.SetQuatRotation(newQuatRotation);
    UpdateVectors();

    for (const auto& attachedComponent : attachedComponents)
    {
        attachedComponent->SetQuatRotation(newQuatRotation);
    }
}

void SceneComponent::Rotate(float rotationDelta, const glm::vec3& axis)
{
    if (MathUtils::IsNearlyZeroVector(axis))
    {
        return;
    }
    
    const glm::quat rot = glm::angleAxis(rotationDelta, glm::normalize(axis));
    transform.SetQuatRotation(transform.GetQuatRotation() * rot);
    
    UpdateVectors();
    
    for (const auto& attachedComponent : attachedComponents)
    {
        attachedComponent->Rotate(rotationDelta, axis);
    }
}

void SceneComponent::Scale(const glm::vec3& scaleDelta)
{
    if (MathUtils::IsNearlyZeroVector(scaleDelta))
    {
        return;
    }
    
    transform.SetScale(transform.GetScale() + scaleDelta);
    UpdateVectors();
    
    for (const auto& attachedComponent : attachedComponents)
    {
        attachedComponent->Scale(scaleDelta);
    }
}

void SceneComponent::SetPosition(const glm::vec3& newPosition)
{
    const auto moveDelta = newPosition - transform.GetPosition();
    Move(moveDelta);
}

void SceneComponent::SetScale(const glm::vec3& newScale)
{
    const auto scaleDelta = newScale - transform.GetScale();
    Scale(scaleDelta);
}

void SceneComponent::UpdateVectors()
{
    forward = glm::normalize(glm::mat3(transform.GetModelMatrix()) * glm::vec3(0.0f, 0.0f, -1.0f));
    
    right = glm::normalize(glm::cross(forward, {0.0f, 1.0f, 0.0f}));
    up = glm::cross(right, forward);
}

void SceneComponent::AttachComponent(SceneComponent* component)
{
    if (!component || component == this)
    {
        return;
    }
    
    for (auto* attachedComponent : attachedComponents)
    {
        if (attachedComponent == component)
        {
            return;
        }
    }
    attachedComponents.push_back(component);
}

void SceneComponent::DetachComponent(SceneComponent* component)
{
    std::erase_if(attachedComponents, [component](const SceneComponent* attachedComponent) {return component == attachedComponent; });
}