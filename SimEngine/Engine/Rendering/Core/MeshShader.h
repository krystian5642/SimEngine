#pragma once
#include "Shader.h"

class MeshShader : public Shader
{
public:
    MeshShader(const ShaderData& shaderData);
    
    void BindAndValidate() const override;
};
