#include "Material.h"
#include "Shader.h"
#include "Texture.h"
    
void Material::Use(const std::shared_ptr<const Shader>& shader) const
{
    const auto& shaderToUse = resources.shader ? resources.shader : shader;
    const auto& data = resources.data;
    
    shaderToUse->SetVec3f(UniformNames::materialAmbient, data.ambient);
    shaderToUse->SetVec3f(UniformNames::materialDiffuse, data.diffuse);
    shaderToUse->SetVec3f(UniformNames::materialSpecular, data.specular);
    shaderToUse->SetFloat(UniformNames::materialShininess, data.shininess);
    shaderToUse->SetBool(UniformNames::materialUseDiffuseTexture, resources.diffuseTexture != nullptr);
    shaderToUse->SetBool(UniformNames::materialUseNormalTexture, resources.normalTexture != nullptr && resources.canUseNormalMap);

    if (resources.diffuseTexture)
    {
        resources.diffuseTexture->Bind();
        shaderToUse->SetInt(UniformNames::materialDiffuseTexture, 0);
    }
    
    if (resources.normalTexture)
    {
        resources.normalTexture->Bind(1);
        shaderToUse->SetInt(UniformNames::materialNormalTexture, 1);
    }
}

MaterialPtr ReflectMaterial::CreateReflectMaterial()
{
    auto rendererAPI = Renderer::Get();
        
    ShaderData shaderData;
    shaderData.fragShader = ShaderData::shadersFolder + "reflect.frag";
    shaderData.vertShader = ShaderData::shadersFolder + "reflect.vert";
        
    MaterialResources resources;
    resources.shader = rendererAPI->CreateShader(shaderData);
        
    return rendererAPI->CreateReflectMaterial(resources);
}

MaterialPtr RefractMaterial::CreateRefractMaterial()
{
    auto rendererAPI = Renderer::Get();
        
    ShaderData shaderData;
    shaderData.fragShader = ShaderData::shadersFolder + "refract.frag";
    shaderData.vertShader = ShaderData::shadersFolder + "refract.vert";
        
    MaterialResources resources;
    resources.shader = rendererAPI->CreateShader(shaderData);
        
    return rendererAPI->CreateRefractMaterial(resources);
}
