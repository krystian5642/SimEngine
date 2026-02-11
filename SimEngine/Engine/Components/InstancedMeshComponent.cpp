#include "InstancedMeshComponent.h"
#include "Rendering/Core/InstancedMesh.h"
#include "Rendering/Core/Shader.h"
#include "Scene/SceneManager.h"
#include "Scene/Scene.h"

InstancedMeshComponent::InstancedMeshComponent(ObjectBase* parent, Scene* scene, const std::string& name)
    : RenderComponent(parent, scene, name)
{
}

void InstancedMeshComponent::Draw(const std::shared_ptr<const Shader>& shader, bool visualPass) const
{
    if (!instancedMesh)
    {
        std::cout << "No mesh specified" << std::endl;
        return;
    }
    
    shader->SetBool(UniformNames::useInstanceRendering, true);
    
    auto activeShader = shader;
    
    if (visualPass && material)
    {
        material->Use(shader);
        auto materialShader = material->GetResources().shader;
        if (materialShader)
        {
            materialShader->Bind();
            activeShader = materialShader;
            
            const auto currentScene = SceneManager::GetCurrentScene();
    
            const auto projection = currentScene->GetProjectionMatrix();
            const auto& view = currentScene->GetViewMatrix();

            materialShader->SetMat4f(UniformNames::projection, projection);
            materialShader->SetMat4f(UniformNames::view, view);
        }
    }
    
    instancedMesh->Draw();
    
    if (activeShader != shader)
    {
        activeShader->Unbind();
    }
    
    shader->SetBool(UniformNames::useInstanceRendering, false);
}
