#include "SpotLightObject.h"

#include <string>

#include "Rendering/Renderer/Renderer.h"
#include "Rendering/Core/Shader.h"

namespace SimEngine
{
    SpotLightObject::SpotLightObject(ObjectBase* parent, Scene* scene, const std::string& name)
        : PointLightObject(parent, scene, name)
    {
    }

    SpotLightObject::~SpotLightObject()
    {
    }

    void SpotLightObject::Use(const std::shared_ptr<const Shader>& shader) const
    {
        const auto index = std::to_string(lightIndex);
        
        const auto spotLightColor = "spotLights[" + index + "].lightData.color";
        const auto spotLightAmbient = "spotLights[" + index + "].lightData.ambient";
        const auto spotLightDiffuse = "spotLights[" + index + "].lightData.diffuse";
    
        const auto spotLightPosition = "spotLights[" + index + "].position";
        const auto spotLightDirection = "spotLights[" + index + "].direction";
        const auto spotLightInnerConeAngle = "spotLights[" + index + "].innerConeAngle";
        const auto spotLightOuterConeAngle = "spotLights[" + index + "].outerConeAngle";
        const auto farPlane = "omniShadowMaps[" + index + "].farPlane";
    
        shader->SetVec3f(spotLightPosition, GetPosition());
        shader->SetVec3f(spotLightDirection, spotLightData.direction);
        shader->SetVec3f(spotLightColor, lightData.color);
        shader->SetFloat(spotLightAmbient, lightData.ambient);
        shader->SetFloat(spotLightDiffuse, lightData.diffuse);
        shader->SetFloat(spotLightInnerConeAngle, glm::radians(spotLightData.innerConeAngle));
        shader->SetFloat(spotLightOuterConeAngle, glm::radians(spotLightData.outerConeAngle));
    
        shader->SetFloat(farPlane, 100.0f);
    
        shader->SetInt("spotLightCount", lightCount);
    }
}
