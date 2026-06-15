#pragma once

struct VertexData
{
    glm::vec3 position;
    glm::vec2 uv;
    glm::vec3 normal;
};

struct MeshData
{
    std::vector<VertexData> vertices; 
    std::vector<unsigned int> indices;
};

class Mesh
{
public:
    Mesh(const MeshData& meshData);
    ~Mesh();
    
    void Draw() const;
    
private:
    GLuint VAO{}, VBO{}, IBO{};
    GLsizei drawCount{}, indicesSize{}, indicesCount{};
};
