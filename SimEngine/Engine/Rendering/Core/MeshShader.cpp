
#include "MeshShader.h"

#include "Scene/SceneManager.h"
#include "Components/CameraComponent.h"
#include "Scene/Objects/Lighting/DirectionalLightObject.h"
#include "Scene/Objects/Lighting/PointLightObject.h"

MeshShader::MeshShader(const ShaderData& shaderData)
    : Shader(shaderData)
{
}

void MeshShader::BindAndValidate() const
{
    Shader::BindAndValidate();
    
    const auto scene = SceneManager::GetCurrentScene();
    
    const auto& viewMatrix = scene->GetActiveCamera()->GetViewMatrix();
    const auto& lightData = scene->GetLightsData();
    const auto& projection = scene->GetProjectionMatrix();
    
    for (auto* dirLight : lightData.dirLights)
    {
        if (dirLight)
        {
            dirLight->Use(this);
        }
    }

    for (auto* pointLight : lightData.pointLights)
    {
        if (pointLight)
        {
            pointLight->Use(this);
        }
    }

    SetMat4f("projection", projection);
    SetMat4f("view", viewMatrix);
    
    Validate();
}
