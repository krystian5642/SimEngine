#include "GLMesh.h"

namespace SimEngine
{
    GLMesh::GLMesh(const MeshData& meshData)
        : Mesh(meshData)
    {
        GLMesh::LoadGPUData(meshData);
    }

    GLMesh::~GLMesh()
    {
        GLMesh::FreeGPUData();
    }

    void GLMesh::Draw() const
    {
        glBindVertexArray(VAO);
        
        if (IBO != 0)
        {
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
            glDrawElements(GL_TRIANGLES, drawCount, GL_UNSIGNED_INT, nullptr);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        }
        else
        {
            glDrawArrays(GL_TRIANGLES, 0, drawCount);
        }
        
        glBindVertexArray(0);
    }

    void GLMesh::LoadGPUData(const MeshData& meshData)
    {
        drawCount = meshData.vertices.size();
        
        glGenVertexArrays(1, &VAO);
        glBindVertexArray(VAO);

        glGenBuffers(1, &VBO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, meshData.vertices.size() * sizeof(float), meshData.vertices.data(), GL_STATIC_DRAW);
    
        const size_t indicesSize = meshData.indices.size() * sizeof(unsigned int);
        if (indicesSize > 0)
        {
            glGenBuffers(1, &IBO);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicesSize * sizeof(unsigned int), meshData.indices.data(), GL_STATIC_DRAW);
            
            drawCount = indicesSize / sizeof(unsigned int);
        }
        
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), nullptr);
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(sizeof(float) * 3));
        glEnableVertexAttribArray(1);

        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(sizeof(float) * 5));
        glEnableVertexAttribArray(2);

        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    void GLMesh::FreeGPUData()
    {
        if (VAO != 0)
        {
            glDeleteVertexArrays(1, &VAO);
            VAO = 0;
        }

        if (VBO != 0)
        {
            glDeleteBuffers(1, &VBO);
            VBO = 0;
        }
    
        if (IBO != 0)
        {
            glDeleteBuffers(1, &IBO);
            IBO = 0;
        }
    }
}
