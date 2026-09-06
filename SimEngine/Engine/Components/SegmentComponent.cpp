#include "SegmentComponent.h"

#include "Managers/MaterialManager.h"
#include "Managers/MeshManager.h"
#include "Scene/Objects/Entities/Entity.h"

SegmentComponent::SegmentComponent(ObjectBase* parent, Scene* scene, const std::string& name)
    : MeshComponent(parent, scene, name)
{
    mesh = MeshManager::Get().GetAssetByName("arrow_cylinder");
    material = MaterialManager::Get().GetAssetByName("ruby");
}

void SegmentComponent::Tick(float deltaTime)
{
    MeshComponent::Tick(deltaTime);
    
    if (useParentLocationAsStart)
    {
        auto actor = dynamic_cast<Entity*>(parent);
        SetStart(actor->GetPosition());
    }
}

void SegmentComponent::SetStart(const glm::vec3& newStart)
{
    SetPosition(newStart);
}

void SegmentComponent::SetEnd(const glm::vec3& newEnd)
{
    const glm::vec3& direction = newEnd - GetPosition();
    const float scale = glm::length(direction);
    
    SetScale({scale, 1.0f, 1.0f});
    
    if (scale > MathUtils::EPS)
    {
        const glm::vec3 rotation = MathUtils::GetRotationFromDirection(direction);
        SetRotation(rotation);
    }
}
