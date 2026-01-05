#pragma once

#include <memory>
#include <GLM/fwd.hpp>
#include <GLM/vec3.hpp>
#include <GLM/gtc/matrix_transform.hpp>

#include "Scene/Objects/Core/SceneObject.h"

namespace SimEngine
{
    class Shader;
    class ShadowMap;

    struct LightData
    {
        glm::vec3 color{1.0f, 1.0f, 1.0f};
        float ambient{1.0f};
        float diffuse{1.0f};
    };

    class LightObjectBase : public SceneObject
    {
    public:
        LightObjectBase(ObjectBase* parent, Scene* scene, const std::string& name);
        virtual ~LightObjectBase() = 0;
    
        virtual void Use(const std::shared_ptr<const Shader>& shader) const = 0;
    
        ShadowMap* GetShadowMap() const { return shadowMap.get(); }
    
        LightData lightData;
    
        size_t lightIndex{};

    protected:
        glm::mat4 lightProjectionMatrix;
        std::shared_ptr<ShadowMap> shadowMap;
    };
}
