#pragma once
#include "Scene/Objects/Entities/Entity.h"

class SpringComponent;
class MeshComponent;

class HarmonicOscillator_MassiveSpring : public Entity
{
public:
    HarmonicOscillator_MassiveSpring(ObjectBase* parent, Scene* scene, const std::string& name);
    
    void Tick(float deltaTime) override;
    
    float GetOffset() const;
    
    struct PhysicsData
    {
        float mass{2.0f};
        float springLength{10.0f};
        float springMass{0.019f};
        float k{10.0f};
        float gravity{-9.81f};
        int N = 5;
    };
    
    void SetPhysicsData(const PhysicsData& newPhysicsData);
    const PhysicsData& GetPhysicsData() const { return physicsData; }
    
private:
    void ClearComponents();
    void RecreateHarmonicOscillator();
    
    PhysicsData physicsData;
    
    struct SpringMassPoint
    {
        MeshComponent* mesh;
        glm::vec3 velocity{0.0f};
        glm::vec3 force{0.0f};
    };
    
    std::vector<SpringMassPoint> springMassPoints;
    std::vector<SpringComponent*> springs;
    
    struct CalculatedData
    {
        float dk{};
        float dl{};
        float dm{};
    };
    
    CalculatedData calculatedData;
};
