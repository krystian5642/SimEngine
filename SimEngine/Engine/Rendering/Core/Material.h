#pragma once

#include "Rendering/UniformNames.h"

class Shader;
class Texture;

namespace UniformNames
{
    UNIFORM_NAME materialColor = "material.color";
}

struct MaterialData
{
    std::shared_ptr<const Shader> shader;
    glm::vec3 color{1.0f};
};

class Material
{
public:
    Material() = default;
    Material(const MaterialData& materialData) : data(materialData) {}

    void Use() const;
    
    MaterialData data;
};