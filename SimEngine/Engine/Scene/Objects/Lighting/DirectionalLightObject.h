#pragma once

#include "Rendering/UniformNames.h"
#include "LightObjectBase.h"

namespace SimEngine
{
    class Scene;
    class ObjectBase;

    namespace UniformNames
    {
        UNIFORM_NAME dirLightDirection = "dirLight.direction";
        UNIFORM_NAME dirLightColor = "dirLight.lightData.color";
        UNIFORM_NAME dirLightAmbient = "dirLight.lightData.ambient";
        UNIFORM_NAME dirLightDiffuse = "dirLight.lightData.diffuse";
    }

    class DirectionalLightObject : public LightObjectBase
    {
    public:
        DirectionalLightObject(ObjectBase* parent, Scene* scene, const std::string& name);
        ~DirectionalLightObject() override;

        void Init() override;
        void OnDestroy() override;
        void Use(const std::shared_ptr<const Shader>& shader) const override;
    
        const glm::mat4& GetViewProjectionMatrix() const { return viewProjectionMatrix; }
        
        const glm::vec3& GetDirection() const { return direction; }
        void SetDirection(const glm::vec3& newDirection);
        
        static inline int lightCount{};
        
    private:
        void UpdateViewProjectionMatrix();
        
        glm::mat4 viewProjectionMatrix;
        glm::vec3 direction{0.0f, -1.0f, 0.0f };
    };

}