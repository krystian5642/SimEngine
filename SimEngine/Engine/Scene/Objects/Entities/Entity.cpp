#include "Entity.h"

#include <GLFW/glfw3.h>

#include "Components/SceneComponent.h"
#include "Core/App.h"

Entity::Entity(ObjectBase* parent, Scene* scene, const std::string& name)
    : SceneObject(parent, scene, name)
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
    
    const auto isPaused = App::currentApp->GetIsPaused();
    
    components.Tick(deltaTime, isPaused);
    childEntities.Tick(deltaTime, isPaused);
}
    
void Entity::OnDestroy()
{
    SceneObject::OnDestroy();
    
    components.OnDestroy();
    childEntities.OnDestroy();
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
    
void Entity::Rotate(const glm::vec3& rotationDelta)
{
    rootComponent->Rotate(rotationDelta);
    
    childEntities.ForEach([rotationDelta](Entity* child, int index)
    {
        child->Rotate(rotationDelta);
    });
}

void Entity::Rotate(const glm::quat& rotationDelta)
{
    rootComponent->Rotate(rotationDelta);
    
    childEntities.ForEach([rotationDelta](Entity* child, int index)
    {
        child->Rotate(rotationDelta);
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
    
void Entity::SetPosition(const glm::vec3& newPosition)
{
    rootComponent->SetPosition(newPosition);
    
    childEntities.ForEach([newPosition](Entity* child, int index)
    {
        child->SetPosition(newPosition);
    });
}
    
void Entity::SetRotation(const glm::vec3& newRotation)
{
    rootComponent->SetRotation(newRotation);
    
    childEntities.ForEach([newRotation](Entity* child, int index)
    {
        child->SetRotation(newRotation);
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

void Entity::SetUseQuaternionsForRotation(bool use)
{
    rootComponent->SetUseQuaternionsForRotation(use);
}

bool Entity::GetUseQuaternionsForRotation() const
{
    return rootComponent->GetUseQuaternionsForRotation();
}
