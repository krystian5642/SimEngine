#pragma once

#include "PointLightObject.h"
#include <string>
#include "Rendering/UniformNames.h"

namespace SimEngine
{
    class ObjectBase;
    class Scene;

    namespace UniformNames
    {
        UNIFORM_NAME spotLightPosition = "spotLight.position";
        UNIFORM_NAME spotLightDirection = "spotLight.direction";
        UNIFORM_NAME spotLightColor = "spotLight.lightData.color";
        UNIFORM_NAME spotLightAmbient = "spotLight.lightData.ambient";
        UNIFORM_NAME spotLightDiffuse = "spotLight.lightData.diffuse";
        UNIFORM_NAME spotLightInnerConeAngle = "spotLight.innerConeAngle";
        UNIFORM_NAME spotLightOuterConeAngle = "spotLight.outerConeAngle";
    }

    struct SpotLightData
    {
        glm::vec3 direction{1.0f, 0.0f, 0.0f};
        float innerConeAngle = 17.0f;
        float outerConeAngle = 20.0f;
    };

    class SpotLightObject : public PointLightObject
    {
    public:
        SpotLightObject(ObjectBase* parent, Scene* scene, const std::string& name);
        ~SpotLightObject() override;
    
        void Use(const std::shared_ptr<const Shader>& shader) const override;
    
        SpotLightData spotLightData;
    };
}
