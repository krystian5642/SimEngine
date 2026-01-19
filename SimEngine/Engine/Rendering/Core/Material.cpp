#include "Material.h"
#include "Shader.h"
#include "Texture.h"

Material::Material(const MaterialResources& resources)
    : resources(resources)
{
}
    
void Material::Use(const std::shared_ptr<const Shader>& shader) const
{
    const auto& shaderToUse = resources.shader ? resources.shader : shader;
    const auto& data = resources.data;
    
    shaderToUse->SetVec3f(UniformNames::materialAmbient, data.ambient);
    shaderToUse->SetVec3f(UniformNames::materialDiffuse, data.diffuse);
    shaderToUse->SetVec3f(UniformNames::materialSpecular, data.specular);
    shaderToUse->SetFloat(UniformNames::materialShininess, data.shininess);
    shaderToUse->SetBool(UniformNames::materialUseTexture, resources.texture != nullptr);

    if (resources.texture)
    {
        resources.texture->Bind();
    }
}
