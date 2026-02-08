#include "TerrainFace.h"

#include "Rendering/Core/Mesh.h"

float EvaluateNoise(const glm::vec3& point, const std::vector<NoiseSettings>& noiseSettings)
{
    float noise = 0.0f;
    for (const auto& settings : noiseSettings)
    {
        noise += glm::clamp(glm::simplex(point * settings.frequency + settings.offset) * settings.strength
            , settings.minValue, settings.maxValue);
    }
    return noise;
}

void TerrainFace::GenerateMeshData(const glm::vec3& localUp, int resolution, const std::vector<NoiseSettings>& noiseSettings
    , MeshData& outMeshData, float& minHeight, float& maxHeight)
{
    const glm::vec3 axisA{ localUp.z, localUp.x, localUp.y };
    const glm::vec3 axisB = glm::cross(localUp, axisA);
    
    auto& vertices = outMeshData.vertices;
    vertices.reserve(resolution * resolution * 8.0f);
    
    auto& indices = outMeshData.indices;
    indices.reserve(6 * (resolution - 1) * (resolution - 1)); 
    
    for (int y = 0; y < resolution; y++)
    {
        for (int x = 0; x < resolution; x++)
        {
            glm::vec2 percent = glm::vec2{static_cast<float>(x), static_cast<float>(y)} / static_cast<float>(resolution - 1);
            
            glm::vec3 vertex = localUp + (percent.x - 0.5f) * 2.0f * axisA + (percent.y - 0.5f) * 2.0f * axisB;
            vertex = glm::normalize(vertex);
            vertex *= 1.0f + EvaluateNoise(vertex, noiseSettings);
            
            minHeight = std::min(minHeight, glm::length(vertex));
            maxHeight = std::max(maxHeight, glm::length(vertex));
            
            vertices.push_back(vertex.x);
            vertices.push_back(vertex.y);
            vertices.push_back(vertex.z);
            
            // uv and tex coords to 0
            for (int i = 0; i < 5; i++)
            {
               vertices.push_back(0.0f);
            }
            
            if (x < resolution - 1 && y < resolution - 1)
            {
                int i = y * resolution + x;
                
                indices.push_back(i);
                indices.push_back(i + resolution + 1);
                indices.push_back(i + resolution);
                
                indices.push_back(i);
                indices.push_back(i + 1);
                indices.push_back(i + resolution + 1);
            }
        }
    }
    
    //calculate average normals
    for (int i = 0; i < indices.size(); i += 3)
    {
        auto index1 = indices[i] * 8;
        auto index2 = indices[i + 1] * 8;
        auto index3 = indices[i + 2] * 8;
        
        auto vertex1x = vertices[index1];
        auto vertex1y = vertices[index1 + 1];
        auto vertex1z = vertices[index1 + 2];
        
        auto vertex2x = vertices[index2];
        auto vertex2y = vertices[index2 + 1];
        auto vertex2z = vertices[index2 + 2];
        
        auto vertex3x = vertices[index3];
        auto vertex3y = vertices[index3 + 1];
        auto vertex3z = vertices[index3 + 2];
        
        glm::vec3 one = glm::vec3{vertex2x - vertex1x, vertex2y - vertex1y, vertex2z - vertex1z};
        glm::vec3 two = glm::vec3{vertex3x - vertex1x, vertex3y - vertex1y, vertex3z - vertex1z};
        
        glm::vec3 normal = glm::cross(one, two);
        
        vertices[index1 + 5] += normal.x;
        vertices[index1 + 6] += normal.y;
        vertices[index1 + 7] += normal.z;
        
        vertices[index2 + 5] += normal.x;
        vertices[index2 + 6] += normal.y;
        vertices[index2 + 7] += normal.z;
        
        vertices[index3 + 5] += normal.x;
        vertices[index3 + 6] += normal.y;
        vertices[index3 + 7] += normal.z;
    }
    
    for (int i = 0; i < vertices.size(); i += 8)
    {
        glm::vec3 normal = glm::normalize(glm::vec3{vertices[i + 5], vertices[i + 6], vertices[i + 7]});
        vertices[i + 5] = normal.x;
        vertices[i + 6] = normal.y;
        vertices[i + 7] = normal.z;
    }
}
