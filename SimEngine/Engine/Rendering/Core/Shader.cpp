#include "Shader.h"

#include "Core/Files/File.h"

Shader::Shader(const ShaderData& shaderData)
{
    Compile(shaderData);
}

Shader::~Shader()
{
}
    
void Shader::BindAndValidate() const
{
    if (programID == 0)
    {
        throw std::runtime_error("ERROR : Shader is not complied !!!");
    }
    
    GLint prevProgram = 0;
    glGetIntegerv(GL_CURRENT_PROGRAM, &prevProgram);
    previousProgramID = static_cast<GLuint>(prevProgram);
    
    glUseProgram(programID);
    
    Validate();
}
    
void Shader::Unbind() const
{
    glUseProgram(previousProgramID);
}

void Shader::Compile(const ShaderData& shaderData)
{
    programID = glCreateProgram();
    if (programID == 0)
    {
        throw std::runtime_error("Can't create shader program");
    }

    auto vertCode = File::ReadFile(shaderData.vertShader);
    AddShader(programID, vertCode.c_str(), GL_VERTEX_SHADER);
    
    auto fragCode = File::ReadFile(shaderData.fragShader);
    AddShader(programID, fragCode.c_str(), GL_FRAGMENT_SHADER);

    glLinkProgram(programID);

    GLint result;
    glGetProgramiv(programID, GL_LINK_STATUS, &result);
    if (result == GL_FALSE)
    {
        GLchar eLog[1024];
        glGetProgramInfoLog(programID, sizeof(eLog), nullptr, eLog);
        throw std::runtime_error(eLog);
    }
}
        
void Shader::Validate() const
{
    GLint result;
    glValidateProgram(programID);
    glGetProgramiv(programID, GL_VALIDATE_STATUS, &result);
    if (result == GL_FALSE)
    {
        GLchar eLog[1024];
        glGetProgramInfoLog(programID, sizeof(eLog), nullptr, eLog);
        throw std::runtime_error(eLog);
    }
}
    
void Shader::SetInt(std::string_view name, int value) const
{
    glUniform1i(GetUniformLocation(name), value);
}
    
void Shader::SetFloat(std::string_view name, float value) const
{
    glUniform1f(GetUniformLocation(name), value);
}
        
void Shader::SetBool(std::string_view name, bool value) const
{
    glUniform1i(GetUniformLocation(name), static_cast<GLint>(value));
}
    
void Shader::SetUInt(std::string_view name, unsigned int value) const
{
    glUniform1ui(GetUniformLocation(name), value);
}
    
void Shader::SetIntArray(std::string_view name, const int* values, size_t count) const
{
    glUniform1iv(GetUniformLocation(name), static_cast<GLsizei>(count), values);
}
    
void Shader::SetFloatArray(std::string_view name, const float* values, size_t count) const
{
    glUniform1fv(GetUniformLocation(name), static_cast<GLsizei>(count), values);
}
    
void Shader::SetBoolArray(std::string_view name, const bool* values, size_t count) const
{
    glUniform1iv(GetUniformLocation(name), static_cast<GLsizei>(count), reinterpret_cast<const GLint*>(values));
}
    
void Shader::SetMat2f(std::string_view name, const glm::mat2& m) const
{
    glUniformMatrix2fv(GetUniformLocation(name), 1, GL_FALSE, glm::value_ptr(m));
}
    
void Shader::SetMat3f(std::string_view name, const glm::mat3& m) const
{
    glUniformMatrix3fv(GetUniformLocation(name), 1, GL_FALSE, glm::value_ptr(m));
}
    
void Shader::SetMat4f(std::string_view name, const glm::mat4& m) const
{
    glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, glm::value_ptr(m));
}
    
void Shader::SetMat4Array(std::string_view name, const glm::mat4* values, size_t count) const
{
    glUniformMatrix4fv(GetUniformLocation(name), static_cast<GLsizei>(count), GL_FALSE, glm::value_ptr(values[0]));
}
    
void Shader::SetVec2f(std::string_view name, const glm::vec2& v) const
{
    glUniform2f(GetUniformLocation(name), v.x, v.y);
}
    
void Shader::SetVec3f(std::string_view name, const glm::vec3& v) const
{
    glUniform3f(GetUniformLocation(name), v.x, v.y, v.z);
}
    
void Shader::SetVec4f(std::string_view name, const glm::vec4& v) const
{
    glUniform4f(GetUniformLocation(name), v.x, v.y, v.z, v.w);
}
    
void Shader::SetVec3Array(std::string_view name, const glm::vec3* values, size_t count) const
{
    glUniform3fv(GetUniformLocation(name), static_cast<GLsizei>(count), glm::value_ptr(values[0]));
}
    
GLint Shader::GetUniformLocation(std::string_view name) const
{
    auto uniformLocation = glGetUniformLocation(programID, name.data());
    if (uniformLocation == -1)
    {
        std::cout << "Trying to get uniform location " << name << " failed" << std::endl;
    }
    return uniformLocation;
}

void Shader::AddShader(GLuint program, const char* shaderCode, GLenum shaderType)
{
    const auto shader = glCreateShader(shaderType);

    glShaderSource(shader, 1, &shaderCode, nullptr);
    glCompileShader(shader);

    GLint result;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE)
    {
        GLchar infoLog[1024];
        glGetShaderInfoLog(shader, 1024, nullptr, infoLog);
        throw std::runtime_error(infoLog);
    }

    glAttachShader(program, shader);
}