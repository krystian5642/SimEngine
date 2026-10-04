#include "Material.h"
#include "Shader.h"

void Material::Use() const
{
    data.shader->SetVec4f(UniformNames::materialColor, data.color);
}
