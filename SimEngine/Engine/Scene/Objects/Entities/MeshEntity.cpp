#include "MeshEntity.h"

#include "Components/MeshComponent.h"

MeshEntity::MeshEntity(ObjectBase* parent, Scene* scene, const std::string& name)
    : Entity(parent, scene, name)
{
    meshComponent = AddComponent<MeshComponent>();
}