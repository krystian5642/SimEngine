
#include "PointLightObject.h"

#include "Rendering/Core/Shader.h"
#include "Rendering/Renderer/Renderer.h"
#include "Scene/Scene.h"

PointLightObject::PointLightObject(ObjectBase* parent, Scene* scene, const std::string& name)
    : LightObjectBase(parent, scene, name)
{
    lightCount++;
    
    shadowMap = Renderer::CreateOmniShadowMapStatic(2024, 2024);

    lightMatrices.resize(6);
    
    lightProjectionMatrix = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 100.0f);
    UpdateLightMatrices();
}
    
PointLightObject::~PointLightObject()
{
    lightCount--;
}
    
void PointLightObject::Init()
{
    scene->RegisterPointLight(this);
}
    
void PointLightObject::OnDestroy()
{
    scene->UnregisterPointLight(this);
}
    
void PointLightObject::Use(const std::shared_ptr<const Shader>& shader) const
{
    const auto index = std::to_string(lightIndex);
    
    const auto pointLightPosition = "pointLights[" + index + "].position";
    const auto pointLightColor = "pointLights[" + index + "].lightData.color";
    const auto pointLightAmbient = "pointLights[" + index + "].lightData.ambient";
    const auto pointLightDiffuse = "pointLights[" + index + "].lightData.diffuse";
    const auto farPlane = "omniShadowMaps[" + index + "].farPlane";
    
    shader->SetVec3f(pointLightPosition, position);
    shader->SetVec3f(pointLightColor, lightData.color);
    shader->SetFloat(pointLightAmbient, lightData.ambient);
    shader->SetFloat(pointLightDiffuse, lightData.diffuse);
    shader->SetFloat(farPlane, 100.0f);
    
    shader->SetInt("pointLightCount", lightCount);
}
    
void PointLightObject::SetPosition(const glm::vec3& newPosition)
{
    position = newPosition;
    UpdateLightMatrices();
}
    
void PointLightObject::UpdateLightMatrices()
{
    //+x, -x
    lightMatrices[0] = lightProjectionMatrix * glm::lookAt(position, position + glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f));
    lightMatrices[1] = lightProjectionMatrix * glm::lookAt(position, position + glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f));
    
    //+y, -y
    lightMatrices[2] = lightProjectionMatrix * glm::lookAt(position, position + glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    lightMatrices[3] = lightProjectionMatrix * glm::lookAt(position, position + glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f));

    //+z, -z
    lightMatrices[4] = lightProjectionMatrix * glm::lookAt(position, position + glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, -1.0f, 0.0f));
    lightMatrices[5] = lightProjectionMatrix * glm::lookAt(position, position + glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, -1.0f, 0.0f));
}
    