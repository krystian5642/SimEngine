
#include "DirectionalLightObject.h"

#include "Scene/Scene.h"
#include "Rendering/Core/Shader.h"

DirectionalLightObject::DirectionalLightObject(ObjectBase* parent, Scene* scene, const std::string& name)
    : LightObjectBase(parent, scene, name)
{
    lightCount++;
    
    lightProjectionMatrix = glm::ortho(-20.0f, 20.0f, -20.0f, 20.0f, 0.1f, 100.0f);
    UpdateViewProjectionMatrix();
}
    
DirectionalLightObject::~DirectionalLightObject()
{
    lightCount--;
}
    
void DirectionalLightObject::Init()
{
    scene->RegisterDirectionalLight(this);
}
    
void DirectionalLightObject::OnDestroy()
{
    scene->UnregisterDirectionalLight(this);
}
    
void DirectionalLightObject::Use(const Shader* shader) const
{
    const auto index = std::to_string(lightIndex);
    
    const auto dirLightDirection = "dirLights[" + index + "].direction";
    const auto dirLightColor = "dirLights[" + index + "].lightData.color";
    const auto dirLightAmbientIntensity = "dirLights[" + index + "].lightData.ambientIntensity";
    const auto dirLightDiffuseIntensity = "dirLights[" + index + "].lightData.diffuseIntensity";

    shader->SetVec3f(dirLightDirection, direction);
    shader->SetVec3f(dirLightColor, lightData.color);
    shader->SetFloat(dirLightAmbientIntensity, lightData.ambientIntensity);
    shader->SetFloat(dirLightDiffuseIntensity, lightData.diffuseIntensity);

    shader->SetInt("dirLightCount", lightCount);
}
    
void DirectionalLightObject::SetDirection(const glm::vec3& newDirection)
{
    direction = newDirection;
    UpdateViewProjectionMatrix();
}
    
void DirectionalLightObject::UpdateViewProjectionMatrix()
{
    viewProjectionMatrix = lightProjectionMatrix * glm::lookAt(-direction,  glm::vec3{0.0f, 0.0f, 0.0f}, glm::vec3(0.0f, 1.0f, 0.0f));
}