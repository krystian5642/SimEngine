#include "GLMaterial.h"

#include "GLSkybox.h"
#include "Rendering/Core/Shader.h"
#include "Scene/SceneManager.h"
#include "Scene/Scene.h"
#include "Components/CameraComponent.h"
#include "Core/App.h"

namespace SimEngine
{
    GLReflectMaterial::GLReflectMaterial(const MaterialResources& resources)
        : Material(resources)
    {
    }

    GLReflectMaterial::~GLReflectMaterial()
    {
    }

    void GLReflectMaterial::Use(const std::shared_ptr<const Shader>& shader) const
    {
        resources.shader->Bind();
        
        const glm::mat4 projection = glm::perspective(glm::radians(45.0f), App::GetCurrentWindow()->GetAspectRatio(), 0.1f, 10000.0f);
        const auto& view = SceneManager::GetCurrentScene()->GetActiveCamera()->GetView();
                
        const auto& cameraPosition = SceneManager::GetCurrentScene()->GetActiveCamera()->GetPosition();
    
        resources.shader->SetMat4f(UniformNames::projection, projection);
        resources.shader->SetMat4f(UniformNames::view, view);
        resources.shader->SetVec3f(UniformNames::cameraPosition, cameraPosition);
                
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, dynamic_cast<const GLSkybox*>(SceneManager::GetCurrentScene()->GetSkybox())->GetTextureId());
        
        resources.shader->Unbind();
    }

    GLRefractMaterial::GLRefractMaterial(const MaterialResources& resources)
        : Material(resources)
    {
    }

    GLRefractMaterial::~GLRefractMaterial()
    {
    }

    void GLRefractMaterial::Use(const std::shared_ptr<const Shader>& shader) const
    {
        resources.shader->Bind();
        
        const glm::mat4 projection = glm::perspective(glm::radians(45.0f), App::GetCurrentWindow()->GetAspectRatio(), 0.1f, 10000.0f);
        const auto& view = SceneManager::GetCurrentScene()->GetActiveCamera()->GetView();
                
        const auto& cameraPosition = SceneManager::GetCurrentScene()->GetActiveCamera()->GetPosition();
    
        resources.shader->SetMat4f(UniformNames::projection, projection);
        resources.shader->SetMat4f(UniformNames::view, view);
        resources.shader->SetVec3f(UniformNames::cameraPosition, cameraPosition);
                
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, dynamic_cast<const GLSkybox*>(SceneManager::GetCurrentScene()->GetSkybox())->GetTextureId());
        
        resources.shader->Unbind();
    }
}

