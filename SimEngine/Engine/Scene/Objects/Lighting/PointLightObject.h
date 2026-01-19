#pragma once

#include "LightObjectBase.h"
#include "Rendering/UniformNames.h"

namespace UniformNames
{
    UNIFORM_NAME pointLightPosition = "pointLight.position";
    UNIFORM_NAME pointLightColor = "pointLight.lightData.color";
    UNIFORM_NAME pointLightAmbientIntensity = "pointLight.lightData.ambient";
    UNIFORM_NAME pointLightDiffuseIntensity = "pointLight.lightData.diffuse";
}

class PointLightObject : public LightObjectBase
{
public:
    PointLightObject(ObjectBase* parent, Scene* scene, const std::string& name);
    ~PointLightObject() override;

    void Init() override;
    void OnDestroy() override;
    void Use(const std::shared_ptr<const Shader>& shader) const override;

    const glm::vec3& GetPosition() const { return position; }
    void SetPosition(const glm::vec3& newPosition);
    
    const std::vector<glm::mat4>& GetLightMatrices() const { return lightMatrices; }

    static inline int lightCount{};
    
private:
    void UpdateLightMatrices();
    
    std::vector<glm::mat4> lightMatrices;
    glm::vec3 position{};
};
