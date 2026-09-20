#include "VectorVisualizerComponent.h"

#include "Core/App.h"
#include "Managers/MeshManager.h"
#include "Rendering/UniformNames.h"
#include "Rendering/Core/Mesh.h"
#include "Rendering/Core/Shader.h"
#include "Scene/SceneManager.h"
#include "Scene/Objects/Entities/Entity.h"

VectorVisualizerComponent::VectorVisualizerComponent(const SceneObjectParams& params)
    : RenderComponent(params)
{
    ShaderData shaderData;
    shaderData.vertShader = ShaderData::shadersFolder + "arrow.vert";
    shaderData.fragShader = ShaderData::shadersFolder + "arrow.frag";
    shader = std::make_shared<Shader>(shaderData);
    
    cylinderMesh = MeshManager::Get().GetAssetByName("arrow_cylinder");
    coneMesh = MeshManager::Get().GetAssetByName("arrow_cone");
}

void VectorVisualizerComponent::Tick(float deltaTime)
{
    RenderComponent::Tick(deltaTime);
    
    if (useParentLocationAsStart)
    {
        auto actor = dynamic_cast<Entity*>(parent);
        SetStart(actor->GetPosition());
    }
}

void VectorVisualizerComponent::Draw() const
{
    auto& renderer = App::Get().renderer;
    
    renderer.EnableBlending(true);
    
    shader->BindAndValidate();
    shader->SetVec4f(UniformNames::arrowColor, color);
    
    const auto currentScene = SceneManager::GetCurrentScene();
    
    const auto& projection = currentScene->GetProjectionMatrix();
    const auto& view = currentScene->GetViewMatrix();

    shader->SetMat4f(UniformNames::projection, projection);
    shader->SetMat4f(UniformNames::view, view);
    
    shader->SetMat4f(UniformNames::model, cylinderTransform.GetModelMatrix());
    cylinderMesh->Draw();
    
    shader->SetMat4f(UniformNames::model, coneTransform.GetModelMatrix());
    coneMesh->Draw();
    
    shader->Unbind();
    
    renderer.EnableBlending(false);
}

void VectorVisualizerComponent::SetStart(const glm::vec3& newStart)
{
    cylinderTransform.SetPosition(newStart);
    coneTransform.SetPosition(newStart);
}

void VectorVisualizerComponent::SetDirection(const glm::vec3& newDirection)
{
    const float scale = scaleLenghtFactor * glm::length(newDirection);
    cylinderTransform.SetScale({scale, scaleFactor, scaleFactor});
        
    const glm::vec3 newRotation = MathUtils::GetRotationFromDirection(newDirection);
    cylinderTransform.SetEulerRotation(newRotation);
    
    auto modelMatrix = glm::mat4(1.0f);
        
    modelMatrix = glm::rotate(modelMatrix, glm::radians(newRotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
    modelMatrix = glm::rotate(modelMatrix, glm::radians(newRotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
    
    glm::vec3 newPosition{scale, 0.0f, 0.0f};
    newPosition = glm::mat3(modelMatrix) * newPosition;
    coneTransform.SetPosition(newPosition + cylinderTransform.GetPosition());
    
    coneTransform.SetEulerRotation(newRotation);
    coneTransform.SetScale({scaleFactor, scaleFactor, scaleFactor});
}
