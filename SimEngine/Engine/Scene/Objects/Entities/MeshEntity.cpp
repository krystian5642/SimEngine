#include "MeshEntity.h"

#include "Components/MeshComponent.h"

MeshEntity::MeshEntity(ObjectBase* parent, Scene* scene, const std::string& name)
    : Entity(parent, scene, name)
{
    meshComponent = AddComponent<MeshComponent>();
}
    
void MeshEntity::SetMesh(const MeshPtr& newMesh)
{
    meshComponent->mesh = newMesh;
}
    
void MeshEntity::SetMaterial(const MaterialPtr& material)
{
    meshComponent->material = material;
}