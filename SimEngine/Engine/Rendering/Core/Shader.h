#pragma once

struct ShaderData
{
    std::string vertShader;
    std::string fragShader;
    
    static inline std::string shadersFolder = "Engine/Rendering/Shaders/";
};

class Shader
{
public:
    Shader(const ShaderData& shaderData);
    ~Shader();
    
    virtual void BindAndValidate() const;
    void Unbind() const;
    
    void Compile(const ShaderData& shaderData);
    void Validate() const;
    
    void SetInt(std::string_view name, int value) const;
    void SetFloat(std::string_view name, float value) const;
    void SetBool(std::string_view name, bool value) const;
    void SetUInt(std::string_view name, unsigned int value) const;
    void SetIntArray(std::string_view name, const int* values, size_t count) const;
    void SetFloatArray(std::string_view name, const float* values, size_t count) const;
    void SetBoolArray(std::string_view name, const bool* values, size_t count) const;

    void SetMat2f(std::string_view name, const glm::mat2& m) const;
    void SetMat3f(std::string_view name, const glm::mat3& m) const;
    void SetMat4f(std::string_view name, const glm::mat4& m) const;
    void SetMat4Array(std::string_view name, const glm::mat4* values, size_t count) const;

    void SetVec2f(std::string_view name, const glm::vec2& v) const;
    void SetVec3f(std::string_view name, const glm::vec3& v) const;
    void SetVec4f(std::string_view name, const glm::vec4& v) const;
    void SetVec3Array(std::string_view name, const glm::vec3* values, size_t count) const;
    
    GLint GetUniformLocation(std::string_view name) const;
private:
    void AddShader(GLuint program, const char* shaderCode, GLenum shaderType);
    
    GLuint programID{};
    mutable GLuint previousProgramID{};
};