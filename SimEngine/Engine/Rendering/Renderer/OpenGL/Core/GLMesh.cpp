#include "GLMesh.h"

GLMesh::GLMesh(const MeshData& meshData)
    : Mesh(meshData)
{
    drawCount = static_cast<GLsizei>(meshData.vertices.size());
    
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, drawCount * sizeof(VertexData), meshData.vertices.data(), GL_STATIC_DRAW);

    indicesCount = static_cast<GLsizei>(meshData.indices.size());
    indicesSize = static_cast<GLsizei>(indicesCount * sizeof(unsigned int));
    if (indicesSize > 0)
    {
        glGenBuffers(1, &IBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicesSize, meshData.indices.data(), GL_STATIC_DRAW);
        
        drawCount = static_cast<GLsizei>(indicesSize / sizeof(unsigned int));
    }
    
    constexpr auto stride = sizeof(VertexData);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)offsetof(VertexData, position));
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride, (void*)offsetof(VertexData, uv));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, stride, (void*)offsetof(VertexData, normal));
    glEnableVertexAttribArray(2);
    
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void*)offsetof(VertexData, tangent));
    glEnableVertexAttribArray(3);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}
    
GLMesh::~GLMesh()
{
    if (VAO != 0)
    {
        glDeleteVertexArrays(1, &VAO);
    }

    if (VBO != 0)
    {
        glDeleteBuffers(1, &VBO);
    }

    if (IBO != 0)
    {
        glDeleteBuffers(1, &IBO);
    }
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