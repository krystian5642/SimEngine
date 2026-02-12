#include "SceneComponent.h"

#include "Core/MathUtils.h"
#include "Rendering/UniformNames.h"
#include "Rendering/Core/Shader.h"

SceneComponent::SceneComponent(ObjectBase* parent, Scene* scene, const std::string& name)
    : RenderComponent(parent, scene, name)
{
    UpdateVectors();
}

void SceneComponent::Draw(const std::shared_ptr<const Shader>& shader, bool visualPass) const
{
    shader->SetMat4f(UniformNames::model, modelMatrix);
}

void SceneComponent::Move(const glm::vec3& moveDelta)
{
    if (MathUtils::IsNearlyZeroVector(moveDelta))
    {
        return;
    }
    
    transform.position += moveDelta;
    UpdateVectors();

    for (const auto& attachedComponent : attachedComponents)
    {
        attachedComponent->Move(moveDelta);
    }
}

void SceneComponent::Rotate(const glm::vec3& rotationDelta)
{
    if (MathUtils::IsNearlyZeroVector(rotationDelta))
    {
        return;
    }
    
    transform.rotation += rotationDelta;
    UpdateVectors();
    
    for (const auto& attachedComponent : attachedComponents)
    {
        attachedComponent->Rotate(rotationDelta);
    }
}

void SceneComponent::Rotate(const glm::quat& rotationDelta)
{
    transform.rotationQuaternion = glm::normalize(rotationDelta * transform.rotationQuaternion);
    UpdateVectors();

    for (const auto& attachedComponent : attachedComponents)
    {
        attachedComponent->Rotate(rotationDelta);
    }
}

void SceneComponent::Scale(const glm::vec3& scaleDelta)
{
    if (MathUtils::IsNearlyZeroVector(scaleDelta))
    {
        return;
    }
    
    transform.scale += scaleDelta;
    UpdateVectors();
    
    for (const auto& attachedComponent : attachedComponents)
    {
        attachedComponent->Scale(scaleDelta);
    }
}

void SceneComponent::SetPosition(const glm::vec3& newPosition)
{
    const auto moveDelta = newPosition - transform.position;
    Move(moveDelta);
}

void SceneComponent::SetRotation(const glm::vec3& newRotation)
{
    const auto rotationDelta = newRotation- transform.rotation;
    Rotate(rotationDelta);
}

void SceneComponent::SetScale(const glm::vec3& newScale)
{
    const auto scaleDelta = newScale - transform.scale;
    Scale(scaleDelta);
}

void SceneComponent::SetUseQuaternionsForRotation(bool use)
{
    transform.useQuaternion = use;
    
    for (const auto& attachedComponent : attachedComponents)
    {
        attachedComponent->SetUseQuaternionsForRotation(use);
    }
}

bool SceneComponent::GetUseQuaternionsForRotation() const
{
    return transform.useQuaternion;
}

void SceneComponent::UpdateVectors()
{
    UpdateModelMatrix();
    
    forward = glm::normalize(glm::mat3(modelMatrix) * glm::vec3(0.0f, 0.0f, -1.0f));
    
    right = glm::normalize(glm::cross(forward, {0.0f, 1.0f, 0.0f}));
    up = glm::cross(right, forward);
}

void SceneComponent::UpdateModelMatrix()
{
    modelMatrix = transform.CalculateModelMatrix();
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