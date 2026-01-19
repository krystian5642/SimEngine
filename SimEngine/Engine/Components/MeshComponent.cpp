#include "MeshComponent.h"

#include "Rendering/Core/Shader.h"
#include "Rendering/Core/Material.h"
#include "Rendering/Core/Mesh.h"

MeshComponent::MeshComponent(ObjectBase* parent, Scene* scene, const std::string& name)
    : SceneComponent(parent, scene, name)
{
}
    
void MeshComponent::Draw(const std::shared_ptr<const Shader>& shader, bool visualPass) const
{
    if (!mesh)
    {
        std::cout << "No mesh specified" << std::endl;
        return;
    }
    
    auto activeShader = shader;
    
    if (visualPass && material)
    {
        material->Use(shader);
        auto materialShader = material->GetResources().shader;
        if (materialShader)
        {
            materialShader->Bind();
            activeShader = materialShader;
        }
    }
    
    SceneComponent::Draw(activeShader, visualPass);
    mesh->Draw();
    
    if (activeShader != shader)
    {
        activeShader->Unbind();
    }
}
