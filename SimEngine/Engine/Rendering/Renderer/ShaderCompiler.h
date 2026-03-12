#pragma once

class ShaderCompiler
{
public:
    static VkShaderModule CompileHLSLShaderToSPIRV(VkDevice device, const std::string& filename);
};
