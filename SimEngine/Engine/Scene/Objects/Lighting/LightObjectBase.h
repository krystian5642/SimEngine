#pragma once

#include "Scene/Objects/Core/SceneObject.h"

class Shader;
class ShadowMap;

struct LightData
{
    glm::vec3 color{1.0f, 1.0f, 1.0f};
    float ambientIntensity{1.0f};
    float diffuseIntensity{1.0f};
};

class LightObjectBase : public SceneObject
{
public:
    using SceneObject::SceneObject;

    virtual void Use(const Shader* shader) const = 0;
    
    LightData lightData;

    size_t lightIndex{};

protected:
    glm::mat4 lightProjectionMatrix;
};
