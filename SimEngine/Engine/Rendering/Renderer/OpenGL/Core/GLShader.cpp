#include "GLShader.h"

#include "Core/Files/File.h"

namespace SimEngine
{
    GLShader::GLShader(const ShaderData& shaderData)
        : Shader(shaderData)
    {
        GLShader::Compile(shaderData);
    }

    GLShader::~GLShader()
    {
    }

    void GLShader::Compile(const ShaderData& shaderData)
    {
        programID = glCreateProgram();
        if (programID == 0)
        {
            throw std::runtime_error("Can't create shader program");
        }

        auto vertCode = File::ReadFile(shaderData.vertShader);
        AddShader(programID, vertCode.c_str(), GL_VERTEX_SHADER);
    
        if (!shaderData.geomShader.empty())
        {
            auto geomCode = File::ReadFile(shaderData.geomShader);
            AddShader(programID, geomCode.c_str(), GL_GEOMETRY_SHADER);
        }
    
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

    void GLShader::Bind() const
    {
        if (programID == 0)
        {
            throw std::runtime_error("ERROR : Shader is not complied !!!");
        }
        
        GLint prevProgram = 0;
        glGetIntegerv(GL_CURRENT_PROGRAM, &prevProgram);
        previousProgramID = static_cast<GLuint>(prevProgram);
        
        glUseProgram(programID);
    }

    void GLShader::Unbind() const
    {
        glUseProgram(previousProgramID);
    }

    void GLShader::Validate() const
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

    void GLShader::SetInt(std::string_view name, int value) const
    {
        glUniform1i(GetUniformLocation(name), value);
    }

    void GLShader::SetFloat(std::string_view name, float value) const
    {
        glUniform1f(GetUniformLocation(name), value);
    }

    void GLShader::SetBool(std::string_view name, bool value) const
    {
        glUniform1i(GetUniformLocation(name), static_cast<GLint>(value));
    }

    void GLShader::SetUInt(std::string_view name, unsigned int value) const
    {
        glUniform1ui(GetUniformLocation(name), value);
    }

    void GLShader::SetIntArray(std::string_view name, const int* values, size_t count) const
    {
        glUniform1iv(GetUniformLocation(name), static_cast<GLsizei>(count), values);
    }

    void GLShader::SetFloatArray(std::string_view name, const float* values, size_t count) const
    {
        glUniform1fv(GetUniformLocation(name), static_cast<GLsizei>(count), values);
    }

    void GLShader::SetBoolArray(std::string_view name, const bool* values, size_t count) const
    {
        glUniform1iv(GetUniformLocation(name), static_cast<GLsizei>(count), reinterpret_cast<const GLint*>(values));
    }

    void GLShader::SetMat2f(std::string_view name, const glm::mat2& m) const
    {
        glUniformMatrix2fv(GetUniformLocation(name), 1, GL_FALSE, glm::value_ptr(m));
    }

    void GLShader::SetMat3f(std::string_view name, const glm::mat3& m) const
    {
        glUniformMatrix3fv(GetUniformLocation(name), 1, GL_FALSE, glm::value_ptr(m));
    }

    void GLShader::SetMat4f(std::string_view name, const glm::mat4& m) const
    {
        glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, glm::value_ptr(m));
    }

    void GLShader::SetMat4Array(std::string_view name, const glm::mat4* values, size_t count) const
    {
        glUniformMatrix4fv(GetUniformLocation(name), static_cast<GLsizei>(count), GL_FALSE, glm::value_ptr(values[0]));
    }

    void GLShader::SetVec2f(std::string_view name, const glm::vec2& v) const
    {
        glUniform2f(GetUniformLocation(name), v.x, v.y);
    }

    void GLShader::SetVec3f(std::string_view name, const glm::vec3& v) const
    {
        glUniform3f(GetUniformLocation(name), v.x, v.y, v.z);
    }

    void GLShader::SetVec4f(std::string_view name, const glm::vec4& v) const
    {
        glUniform4f(GetUniformLocation(name), v.x, v.y, v.z, v.w);
    }

    void GLShader::SetVec3Array(std::string_view name, const glm::vec3* values, size_t count) const
    {
        glUniform3fv(GetUniformLocation(name), static_cast<GLsizei>(count), glm::value_ptr(values[0]));
    }

    GLint GLShader::GetUniformLocation(std::string_view name) const
    {
        auto uniformLocation = glGetUniformLocation(programID, name.data());
        if (uniformLocation == -1)
        {
            std::cout <<"Trying to get uniform location " << name << " failed" << std::endl;
        }
        return uniformLocation;
    }

    void GLShader::AddShader(GLuint program, const char* shaderCode, GLenum shaderType)
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
}
