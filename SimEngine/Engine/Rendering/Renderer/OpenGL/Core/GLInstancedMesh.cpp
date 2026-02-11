#include "GLInstancedMesh.h"

#include "GLMesh.h"
#include "Rendering/Core/Mesh.h"

GLInstancedMesh::GLInstancedMesh(MeshPtr mesh, const std::vector<Transform>& transforms)
    : InstancedMesh(mesh, transforms)
{
    glMesh = dynamic_pointer_cast<const GLMesh>(mesh);
    InitInstanceBuffer();
}

GLInstancedMesh::~GLInstancedMesh()
{
    if (instanceVBO != 0)
    {
        glDeleteBuffers(1, &instanceVBO);
    }
}

void GLInstancedMesh::SetTransforms(const std::vector<Transform>& newTransforms)
{
    InstancedMesh::SetTransforms(newTransforms);
    
    InitInstanceBuffer();
}

void GLInstancedMesh::AddTransform(const Transform& transform)
{
    InstancedMesh::AddTransform(transform);
    
    InitInstanceBuffer();
}

void GLInstancedMesh::Draw() const
{
    const auto indicesCount = glMesh->GetIndicesCount();
    const auto instanceCount = static_cast<GLsizei>(transformMatrices.size());
    
    glBindVertexArray(glMesh->GetVAO());
    glDrawElementsInstanced(GL_TRIANGLES, indicesCount, GL_UNSIGNED_INT, nullptr, instanceCount);
}

void GLInstancedMesh::InitInstanceBuffer()
{
    if (instanceVBO != 0)
    {
        glDeleteBuffers(1, &instanceVBO);
        instanceVBO = 0;
    }
    
    glBindVertexArray(glMesh->GetVAO());
    
    const auto instanceCount = static_cast<GLsizei>(transformMatrices.size());
    
    glGenBuffers(1, &instanceVBO);
    glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
    glBufferData(GL_ARRAY_BUFFER, instanceCount * sizeof(glm::mat4), transformMatrices.data(), GL_STATIC_DRAW);
    
    constexpr auto vec4Size = sizeof(glm::vec4);
    constexpr auto stride = 4 * vec4Size;
    for (int i = 0; i < 4; i++)
    {
        glEnableVertexAttribArray(3 + i);
        glVertexAttribPointer(3 + i, 4, GL_FLOAT, GL_FALSE, stride, (void*)(i * vec4Size));
        glVertexAttribDivisor(3 + i, 1);
    }
    
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}


