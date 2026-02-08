#pragma once

struct MeshData;

struct NoiseSettings
{
    float strength{1.0f};
    float frequency{1.0f};
    float minValue{0.0f};
    float maxValue{1.0f};
    glm::vec3 offset{0.0f};
};

class TerrainFace
{
public:
    static void GenerateMeshData(
        const glm::vec3& localUp
        , int resolution
        , const std::vector<NoiseSettings>& noiseSettings
        , MeshData& outMeshData
        , float& minHeight
        , float& maxHeight);
};
