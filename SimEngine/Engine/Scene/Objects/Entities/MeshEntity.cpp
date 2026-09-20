#include "MeshEntity.h"

#include "Components/MeshComponent.h"

MeshEntity::MeshEntity(const SceneObjectParams& params)
    : Entity(params)
{
    meshComponent = AddComponent<MeshComponent>();
}