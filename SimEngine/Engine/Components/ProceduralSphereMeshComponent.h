#pragma once

#include "Components/MeshComponent.h"

struct NoiseSettings
{
    float strength{1.0f};
    float frequency{1.0f};
    float minValue{0.0f};
    float maxValue{1.0f};
    glm::vec3 offset{0.0f};
};

class ProceduralSphereMeshComponent : public MeshComponent
{
public:
    ProceduralSphereMeshComponent(ObjectBase* parent, Scene* scene, const std::string& name);

    void SetNoiseSettings(const std::vector<NoiseSettings>& noiseSettings);
    const std::vector<NoiseSettings>& GetNoiseSettings() const { return noiseSettings; }
    
    void SetResolution(unsigned int resolution);
    
private:
    std::vector<NoiseSettings> noiseSettings;
    unsigned int resolution{100};
    
    float EvaluateNoise(const glm::vec3& point) const;
    void GenerateMesh();
};
