#include "GLMaterial.h"

#include "GLSkybox.h"
#include "Rendering/Core/Shader.h"
#include "Scene/SceneManager.h"
#include "Scene/Scene.h"
#include "Components/CameraComponent.h"
#include "Core/App.h"

GLReflectMaterial::GLReflectMaterial(const MaterialResources& resources)
    : Material(resources)
{
}

void GLReflectMaterial::Use(const std::shared_ptr<const Shader>& shader) const
{
    resources.shader->Bind();
    
    const auto currentScene = SceneManager::GetCurrentScene();
    
    const auto projection = currentScene->GetProjectionMatrix();
    const auto& view = currentScene->GetViewMatrix();
    const auto& cameraPosition = currentScene->GetCameraPosition();

    resources.shader->SetMat4f(UniformNames::projection, projection);
    resources.shader->SetMat4f(UniformNames::view, view);
    resources.shader->SetVec3f(UniformNames::cameraPosition, cameraPosition);
            
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, dynamic_cast<const GLSkybox*>(currentScene->GetSkybox())->GetTextureId());
    
    resources.shader->Unbind();
}

GLRefractMaterial::GLRefractMaterial(const MaterialResources& resources)
    : Material(resources)
{
}

void GLRefractMaterial::Use(const std::shared_ptr<const Shader>& shader) const
{
    resources.shader->Bind();
    
    const auto currentScene = SceneManager::GetCurrentScene();
    
    const auto projection = currentScene->GetProjectionMatrix();
    const auto& view = currentScene->GetViewMatrix();
    const auto& cameraPosition = currentScene->GetCameraPosition();

    resources.shader->SetMat4f(UniformNames::projection, projection);
    resources.shader->SetMat4f(UniformNames::view, view);
    resources.shader->SetVec3f(UniformNames::cameraPosition, cameraPosition);
            
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, dynamic_cast<const GLSkybox*>(currentScene->GetSkybox())->GetTextureId());
    
    resources.shader->Unbind();
}

