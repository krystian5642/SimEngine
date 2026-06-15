#include "Material.h"
#include "Shader.h"

void Material::Use() const
{
    data.shader->SetVec3f(UniformNames::materialColor, data.color);
}
