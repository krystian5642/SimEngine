#include "ProceduralSphereMeshComponent.h"

#include "Core/MathUtils.h"
#include "Rendering/Core/Mesh.h"

ProceduralSphereMeshComponent::ProceduralSphereMeshComponent(ObjectBase* parent, Scene* scene, const std::string& name)
    : MeshComponent(parent, scene, name)
{
    GenerateMesh();
}

void ProceduralSphereMeshComponent::SetNoiseSettings(const std::vector<NoiseSettings>& newNoiseSettings)
{
    noiseSettings = newNoiseSettings;
    GenerateMesh();
}

void ProceduralSphereMeshComponent::SetResolution(unsigned int newResolution)
{
    resolution = newResolution;
    GenerateMesh();
}

float ProceduralSphereMeshComponent::EvaluateNoise(const glm::vec3& point) const
{
    float noise = 0.0f;
    for (const auto& settings : noiseSettings)
    {
        noise += glm::clamp(glm::simplex(point * settings.frequency + settings.offset) * settings.strength
            , settings.minValue, settings.maxValue);
    }
    return noise;
}

void ProceduralSphereMeshComponent::GenerateMesh()
{
    float minHeight{INFINITY};
    float maxHeight{-INFINITY};
    
    MeshData meshData;
    auto& vertices = meshData.vertices;
    vertices.reserve((resolution + 1) * (resolution + 1) * 6);
        
    auto& indices = meshData.indices;
    indices.reserve(36 * resolution * resolution); 
    
    for (int i = 0; i < MathUtils::directions.size(); i++)
    {
        const auto& localUp = MathUtils::directions[i];
        
        const glm::vec3 axisA{ localUp.z, localUp.x, localUp.y };
        const glm::vec3 axisB = glm::cross(localUp, axisA);
        
        for (int y = 0; y < resolution; y++)
        {
            for (int x = 0; x < resolution; x++)
            {
                VertexData vertexData;
                
                const auto percent = glm::vec2{static_cast<float>(x), static_cast<float>(y)} / static_cast<float>(resolution - 1);
                
                vertexData.position = localUp + (percent.x - 0.5f) * 2.0f * axisA + (percent.y - 0.5f) * 2.0f * axisB;
                vertexData.position = glm::normalize(vertexData.position);
                vertexData.position *= 1.0f + EvaluateNoise(vertexData.position);
                
                minHeight = std::min(minHeight, glm::length(vertexData.position));
                maxHeight = std::max(maxHeight, glm::length(vertexData.position));
                
                const auto offset = i * (resolution * resolution);
                if (x < resolution - 1 && y < resolution - 1)
                {
                    const auto index = offset + y * resolution + x;
                    
                    indices.push_back(index);
                    indices.push_back(index + resolution + 1);
                    indices.push_back(index + resolution);
                    
                    indices.push_back(index);
                    indices.push_back(index + 1);
                    indices.push_back(index + resolution + 1);
                }
                
                vertices.push_back(vertexData);
            }
        }
    }
    
    //calculate average normals
    for (int i = 0; i < indices.size(); i += 3)
    {
        const auto normal = glm::normalize(glm::cross(vertices[indices[i + 1]].position - vertices[indices[i]].position
            , vertices[indices[i + 2]].position - vertices[indices[i]].position));
        
        vertices[indices[i]].position = normal;
        vertices[indices[i + 1]].position = normal;
        vertices[indices[i + 2]].position = normal;
    }
        
    
    for (auto& vertex : vertices)
    {
        // normalize normals
        vertex.normal = glm::normalize(vertex.normal);
        
        // update uv
        const auto length = glm::length(vertex.position);
        
        vertex.uv.x = (length - minHeight) / (maxHeight - minHeight);
    }
    
    mesh = Mesh::CreateMesh(meshData);
}
