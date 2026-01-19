#include "MeshEntity.h"

#include "Components/MeshComponent.h"

MeshEntity::MeshEntity(ObjectBase* parent, Scene* scene, const std::string& name)
    : Entity(parent, scene, name)
{
    meshComponent = AddComponent<MeshComponent>();
}
    
void MeshEntity::SetMesh(const std::shared_ptr<Mesh>& newMesh)
{
    meshComponent->mesh = newMesh;
}
    
void MeshEntity::SetMaterial(const std::shared_ptr<Material>& material)
{
    meshComponent->material = material;
}