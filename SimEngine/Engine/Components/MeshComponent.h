#pragma once

#include "RenderComponent.h"
#include "SceneComponent.h"

namespace SimEngine
{
    class Mesh;
    class Material;

    class MeshComponent : public SceneComponent
    {
    public:
        MeshComponent(ObjectBase* parent, Scene* scene, const std::string& name);
        ~MeshComponent() override;
    
        void Draw(const std::shared_ptr<const Shader>& shader, bool visualPass) const override;
        
        std::shared_ptr<Mesh> mesh;
        std::shared_ptr<Material> material;
    };
}
