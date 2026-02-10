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
    vertices.reserve(resolution * resolution * 48);
        
    auto& indices = meshData.indices;
    indices.reserve(36 * (resolution - 1) * (resolution - 1)); 
    
    for (int i = 0; i < MathUtils::directions.size(); i++)
    {
        const auto& localUp = MathUtils::directions[i];
        
        const glm::vec3 axisA{ localUp.z, localUp.x, localUp.y };
        const glm::vec3 axisB = glm::cross(localUp, axisA);
        
        for (int y = 0; y < resolution; y++)
        {
            for (int x = 0; x < resolution; x++)
            {
                const auto percent = glm::vec2{static_cast<float>(x), static_cast<float>(y)} / static_cast<float>(resolution - 1);
                
                auto vertex = localUp + (percent.x - 0.5f) * 2.0f * axisA + (percent.y - 0.5f) * 2.0f * axisB;
                vertex = glm::normalize(vertex);
                vertex *= 1.0f + EvaluateNoise(vertex);
                
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
            }
        }
    }
    
    //calculate average normals
    for (int i = 0; i < indices.size(); i += 3)
    {
        const unsigned int idx[3] = { indices[i] * 8, indices[i + 1] * 8, indices[i + 2] * 8};
            
        const glm::vec3 v[3] = {
            glm::vec3(vertices[idx[0]], vertices[idx[0]+1], vertices[idx[0]+2]),
            glm::vec3(vertices[idx[1]], vertices[idx[1]+1], vertices[idx[1]+2]),
            glm::vec3(vertices[idx[2]], vertices[idx[2]+1], vertices[idx[2]+2])
        };
            
        const auto normal = glm::normalize(glm::cross(v[1] - v[0], v[2] - v[0]));
        
        for (const auto& index : idx)
        {
            vertices[index+5] += normal.x;
            vertices[index+6] += normal.y;
            vertices[index+7] += normal.z;
        }
    }
        
    for (int i = 0; i < vertices.size(); i += 8)
    {
        // normalize normals
        const auto normal = glm::normalize(glm::vec3{vertices[i + 5], vertices[i + 6], vertices[i + 7]});
        vertices[i + 5] = normal.x;
        vertices[i + 6] = normal.y;
        vertices[i + 7] = normal.z;
        
        // update uv
        const auto vertex = glm::vec3{vertices[i], vertices[i + 1], vertices[i + 2]};
        const auto length = glm::length(vertex);
        
        float& u = vertices[i + 3];
        
        u = (length - minHeight) / (maxHeight - minHeight);
    }
    
    mesh = Mesh::CreateMesh(meshData);
}
