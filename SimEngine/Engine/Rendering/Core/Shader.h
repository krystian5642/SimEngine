#pragma once

#include <string>
#include <GLM/fwd.hpp>

namespace SimEngine
{
    struct ShaderData
    {
        std::string vertShader;
        std::string geomShader;
        std::string fragShader;
    };
    
    class Shader
    {
    public:
        Shader(const ShaderData& shaderData);
        virtual ~Shader() = 0;
        
        virtual void Compile(const ShaderData& shaderData) = 0;
        
        virtual void Bind() const = 0;
        virtual void Unbind() const = 0;
        
        virtual void Validate() const = 0;

        virtual void SetInt(std::string_view name, int value) const = 0;
        virtual void SetFloat(std::string_view name, float value) const = 0;
        virtual void SetBool(std::string_view name, bool value) const = 0;
        virtual void SetUInt(std::string_view name, unsigned int value) const = 0;
        virtual void SetIntArray(std::string_view name, const int* values, size_t count) const = 0;
        virtual void SetFloatArray(std::string_view name, const float* values, size_t count) const = 0;
        virtual void SetBoolArray(std::string_view name, const bool* values, size_t count) const = 0;
        
        virtual void SetMat2f(std::string_view name, const glm::mat2& m) const = 0;
        virtual void SetMat3f(std::string_view name, const glm::mat3& m) const = 0;
        virtual void SetMat4f(std::string_view name, const glm::mat4& m) const = 0;
        virtual void SetMat4Array(std::string_view name, const glm::mat4* values, size_t count) const = 0;
        
        virtual void SetVec2f(std::string_view name, const glm::vec2& v) const = 0;
        virtual void SetVec3f(std::string_view name, const glm::vec3& v) const = 0;
        virtual void SetVec4f(std::string_view name, const glm::vec4& v) const = 0;
        virtual void SetVec3Array(std::string_view name, const glm::vec3* values, size_t count) const = 0;
    };
}
