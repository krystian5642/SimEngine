#include "MeshComponent.h"

#include "Rendering/Core/Shader.h"
#include "Rendering/Core/Material.h"
#include "Rendering/Core/Mesh.h"
#include "Scene/SceneManager.h"
#include "Scene/Scene.h"

MeshComponent::MeshComponent(ObjectBase* parent, Scene* scene, const std::string& name)
    : SceneComponent(parent, scene, name)
{
}
    
void MeshComponent::Draw() const
{
    if (!mesh)
    {
        std::cout << "No mesh specified" << std::endl;
        return;
    }
    
    if (!material)
    {
        std::cout << "No material specified" << std::endl;
        return;
    }
    
    const auto& shader = material->data.shader;
    if (!shader)
    {
        std::cout << "No shader specified" << std::endl;
        return;
    }
    
    shader->BindAndValidate();
        
    shader->SetMat4f(UniformNames::model, GetModelMatrix());
    material->Use();
    
    const auto currentScene = SceneManager::GetCurrentScene();
    
    const auto& projection = currentScene->GetProjectionMatrix();
    const auto& view = currentScene->GetViewMatrix();

    shader->SetMat4f(UniformNames::projection, projection);
    shader->SetMat4f(UniformNames::view, view);
    
    mesh->Draw();
    
    shader->Unbind();
}
