#include "Entity.h"

#include <GLFW/glfw3.h>

#include "imgui.h"
#include "Components/SceneComponent.h"
#include "Core/App.h"

Entity::Entity(const SceneObjectParams& params)
    : SceneObject(params)
{
    rootComponent = AddComponent<SceneComponent>("root");
}

void Entity::Init()
{
    SceneObject::Init();
    
    components.Init();
    childEntities.Init();
}

void Entity::Start()
{
    SceneObject::Start();
    
    components.Start();
    childEntities.Start();
}

void Entity::Tick(float deltaTime)
{
    SceneObject::Tick(deltaTime);
    
    const auto isPaused = App::Get().isPaused;
    
    components.Tick(deltaTime, isPaused);
    childEntities.Tick(deltaTime, isPaused);
}
    
void Entity::OnDestroy()
{
    SceneObject::OnDestroy();
    
    components.OnDestroy();
    childEntities.OnDestroy();
}

void Entity::DrawUI()
{
    SceneObject::DrawUI();
    
    components.ForEach([](Component* component, int index)
    {
        const std::string& name = component->GetName();
        const std::string label = name.empty() ? ("Component " + std::to_string(index)) : name;

        ImGui::PushID(index);
        
        if (component->openUIByDefault)
        {
            ImGui::SetNextItemOpen(true, ImGuiCond_Once);
        }
        
        if (ImGui::TreeNode(label.c_str()))
        {
            component->DrawUI();
            ImGui::TreePop();
        }
        ImGui::PopID();
    });
}

void Entity::DestroyChild(ObjectBase* child)
{
    auto* entity = dynamic_cast<Entity*>(child);
    if (entity)
    {
        childEntities.DestroyObject(entity);
    }
    else
    {
        components.DestroyObject(child);
    }
}
    
void Entity::Move(const glm::vec3& moveDelta)
{
    if (!CanMove())
    {
        return;
    }
    
    rootComponent->Move(moveDelta);
    
    childEntities.ForEach([moveDelta](Entity* child, int index)
    {
        child->Move(moveDelta);
    });
}

void Entity::Rotate(const glm::vec3& rotateDelta)
{
    if (MathUtils::IsNearlyZeroVector(rotateDelta))
    {
        return;    
    }
    
    rootComponent->Rotate(rotateDelta);
    
    childEntities.ForEach([rotateDelta](Entity* child, int index)
    {
        child->Rotate(rotateDelta);
    });
}

void Entity::Rotate(float rotationDelta, const glm::vec3& axis)
{
    if (MathUtils::IsNearlyZeroVector(axis))
    {
        return;
    }
    
    rootComponent->Rotate(rotationDelta, axis);
    
    childEntities.ForEach([rotationDelta, axis](Entity* child, int index)
    {
        child->Rotate(rotationDelta, axis);
    });
}

void Entity::Scale(const glm::vec3& scaleDelta)
{
    rootComponent->Scale(scaleDelta);
    
    childEntities.ForEach([scaleDelta](Entity* child, int index)
    {
        child->Scale(scaleDelta);
    });
}
    
void Entity::SetRotationMode(RotationMode newRotationMode)
{
    rootComponent->SetRotationMode(newRotationMode);
    
    childEntities.ForEach([newRotationMode](Entity* child, int index)
    {
        child->SetRotationMode(newRotationMode);
    });
}

void Entity::SetPosition(const glm::vec3& newPosition)
{
    rootComponent->SetPosition(newPosition);
    
    childEntities.ForEach([newPosition](Entity* child, int index)
    {
        child->SetPosition(newPosition);
    });
}

void Entity::SetEulerRotation(const glm::vec3& newRotation)
{
    rootComponent->SetEulerRotation(newRotation);
    
    childEntities.ForEach([newRotation](Entity* child, int index)
    {
        child->SetEulerRotation(newRotation);
    });
}

void Entity::SetQuatRotation(const glm::quat& newQuatRotation)
{
    rootComponent->SetQuatRotation(newQuatRotation);
    
    childEntities.ForEach([newQuatRotation](Entity* child, int index)
    {
        child->SetQuatRotation(newQuatRotation);
    });
}

void Entity::SetScale(const glm::vec3& newScale)
{
    rootComponent->SetScale(newScale);
    
    childEntities.ForEach([newScale](Entity* child, int index)
    {
        child->SetScale(newScale);
    });
}