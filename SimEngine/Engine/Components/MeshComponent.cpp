#include "MeshComponent.h"

#include "Rendering/Core/Shader.h"
#include "Rendering/Core/Material.h"
#include "Rendering/Core/Mesh.h"

namespace SimEngine
{
    MeshComponent::MeshComponent(ObjectBase* parent, Scene* scene, const std::string& name)
        : SceneComponent(parent, scene, name)
    {
    }

    MeshComponent::~MeshComponent()
    {
    }

    void MeshComponent::Draw(const std::shared_ptr<const Shader>& shader) const
    {
        if (!mesh)
        {
            std::cout << "No mesh specified" << std::endl;
            return;
        }
        
        if (material)
        {
            material->Use(shader);
        }
    
        SceneComponent::Draw(shader);
        mesh->Draw();
    }
}
