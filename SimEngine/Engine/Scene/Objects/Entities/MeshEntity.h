#pragma once

#include "Entity.h"
#include "Components/MeshComponent.h"

namespace SimEngine
{
    class Material;
    class Mesh;
    
    class MeshEntity : public Entity
    {
    public:
        MeshEntity(ObjectBase* parent, Scene* scene, const std::string& name);
        ~MeshEntity() override;
        
        void SetMesh(const std::shared_ptr<Mesh>& newMesh);
        void SetMaterial(const std::shared_ptr<Material>& material);
    
    protected:
        MeshComponent* meshComponent;
    };
}