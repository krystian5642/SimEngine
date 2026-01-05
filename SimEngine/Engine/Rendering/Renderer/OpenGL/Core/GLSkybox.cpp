#include "GLSkybox.h"

#include "Managers/MeshManager.h"
#include "Rendering/UniformNames.h"

#include "stb_image.h"
#include "Rendering/Renderer/Renderer.h"

namespace SimEngine
{
    GLSkybox::GLSkybox(const std::vector<std::string>& faceLocations)
        : Skybox(faceLocations)
    {
        GLSkybox::Create(faceLocations);
    }

    GLSkybox::~GLSkybox()
    {
    }

    void GLSkybox::Draw(const glm::mat4& projectionMatrix, const glm::mat4& viewMatrix) const
    {
        auto newViewMatrix = glm::mat4(glm::mat3(viewMatrix));
    
        glDepthFunc(GL_LEQUAL);
        shader->Bind();
    
        shader->SetMat4f(UniformNames::projection, projectionMatrix);
        shader->SetMat4f(UniformNames::view, newViewMatrix);
    
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, textureId);
    
        shader->Validate();
        mesh->Draw();
    
        shader->Unbind();
        glDepthFunc(GL_LESS);
    }

    void GLSkybox::Create(const std::vector<std::string>& faceLocations)
    {
        const std::string shadersFolder = "Engine/Rendering/Shaders/";
        
        ShaderData skyboxShaderData;
        skyboxShaderData.fragShader = shadersFolder + "skybox.frag";
        skyboxShaderData.vertShader = shadersFolder + "skybox.vert";
        
        shader = Renderer::CreateShaderStatic(skyboxShaderData);
        mesh = MeshManager::Get().GetAssetByName("skybox");
        
        glGenTextures(1, &textureId);
        glBindTexture(GL_TEXTURE_CUBE_MAP, textureId);
    
        int width, height, bitDepth;
    
        for (size_t i = 0; i < faceLocations.size(); i++)
        {
            unsigned char* textureData = stbi_load(faceLocations[i].c_str(), &width, &height, &bitDepth, 0);
            if (!textureData)
            {
                throw std::runtime_error("Failed to load texture " + faceLocations[i]);
            }
        
            glTexImage2D(static_cast<GLenum>(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i), 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, textureData);
            stbi_image_free(textureData);
        
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        }
    }
}
