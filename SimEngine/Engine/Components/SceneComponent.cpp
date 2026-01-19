#include "SceneComponent.h"

#include "Rendering/UniformNames.h"
#include "Rendering/Core/Shader.h"

SceneComponent::SceneComponent(ObjectBase* parent, Scene* scene, const std::string& name)
    : RenderComponent(parent, scene, name)
{
    UpdateForwardVector();
}

void SceneComponent::Draw(const std::shared_ptr<const Shader>& shader, bool visualPass) const
{
    shader->SetMat4f(UniformNames::model, modelMatrix);
}

void SceneComponent::Move(const glm::vec3& moveDelta)
{
    SetPosition(transform.position + moveDelta);
    for (const auto& attachedComponent : attachedComponents)
    {
        attachedComponent->Move(moveDelta);
    }
}

void SceneComponent::Rotate(const glm::vec3& rotationDelta)
{
    SetRotation(transform.rotation + rotationDelta);
    for (const auto& attachedComponent : attachedComponents)
    {
        attachedComponent->Rotate(rotationDelta);
    }
}

void SceneComponent::Scale(const glm::vec3& scaleDelta)
{
    SetScale(transform.scale + scaleDelta);
    for (const auto& attachedComponent : attachedComponents)
    {
        attachedComponent->Scale(scaleDelta);
    }
}

void SceneComponent::SetPosition(const glm::vec3& newPosition)
{
    transform.position = newPosition;
    for (const auto& attachedComponent : attachedComponents)
    {
        attachedComponent->SetPosition(newPosition);
    }
    UpdateForwardVector();
}

void SceneComponent::SetRotation(const glm::vec3& newRotation)
{
    transform.rotation = glm::mod(newRotation, glm::vec3(360.0f));
    for (const auto& attachedComponent : attachedComponents)
    {
        attachedComponent->SetRotation(newRotation);
    }
    UpdateForwardVector();
}

void SceneComponent::SetScale(const glm::vec3& newScale)
{
    transform.scale = newScale;
    UpdateForwardVector();
    
    for (const auto& attachedComponent : attachedComponents)
    {
        attachedComponent->SetScale(newScale);
    }
}

void SceneComponent::UpdateForwardVector()
{
    UpdateModelMatrix();
    forward = glm::normalize(glm::mat3(modelMatrix) * glm::vec3(0.0f, 0.0f, -1.0f));
}

void SceneComponent::UpdateModelMatrix()
{
    modelMatrix = glm::mat4(1.0f);

    modelMatrix = glm::translate(modelMatrix, transform.position);
    modelMatrix = glm::rotate(modelMatrix, glm::radians(transform.rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
    modelMatrix = glm::rotate(modelMatrix, glm::radians(transform.rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
    modelMatrix = glm::rotate(modelMatrix, glm::radians(transform.rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));

    modelMatrix = glm::scale(modelMatrix, transform.scale);
}

void SceneComponent::AttachComponent(SceneComponent* component)
{
    if (component == this)
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