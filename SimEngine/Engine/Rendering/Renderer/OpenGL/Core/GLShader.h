#pragma once

#include "Rendering/Core/Shader.h"

#include <GL/glew.h>

namespace SimEngine
{
    class GLShader : public Shader
    {
    public:
        GLShader(const ShaderData& shaderData);
        ~GLShader() override;
        
        void Compile(const ShaderData& shaderData) override;
        
        void Bind() const override;
        void Unbind() const override;
        
        void Validate() const override;
        
        void SetInt(std::string_view name, int value) const override;
        void SetFloat(std::string_view name, float value) const override;
        void SetBool(std::string_view name, bool value) const override;
        void SetUInt(std::string_view name, unsigned int value) const override;
        void SetIntArray(std::string_view name, const int* values, size_t count) const override;
        void SetFloatArray(std::string_view name, const float* values, size_t count) const override;
        void SetBoolArray(std::string_view name, const bool* values, size_t count) const override;

        void SetMat2f(std::string_view name, const glm::mat2& m) const override;
        void SetMat3f(std::string_view name, const glm::mat3& m) const override;
        void SetMat4f(std::string_view name, const glm::mat4& m) const override;
        void SetMat4Array(std::string_view name, const glm::mat4* values, size_t count) const override;

        void SetVec2f(std::string_view name, const glm::vec2& v) const override;
        void SetVec3f(std::string_view name, const glm::vec3& v) const override;
        void SetVec4f(std::string_view name, const glm::vec4& v) const override;
        void SetVec3Array(std::string_view name, const glm::vec3* values, size_t count) const override;
        
        GLint GetUniformLocation(std::string_view name) const;
    private:
        void AddShader(GLuint program, const char* shaderCode, GLenum shaderType);
        
        GLuint shaderID{};
    };
}
