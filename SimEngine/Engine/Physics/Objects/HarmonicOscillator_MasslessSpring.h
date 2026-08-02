#pragma once

#include "Scene/Objects/Entities/Entity.h"

class MeshComponent;
class SpringComponent;
class VectorVisualizerComponent;

class HarmonicOscillator_MasslessSpring : public Entity
{
public:
    HarmonicOscillator_MasslessSpring(ObjectBase* parent, Scene* scene, const std::string& name);
    
    void Tick(float deltaTime) override;
    
    float GetOffset() const;
    
    struct PhysicsData
    {
        float mass{2.0f};
        float springLength{5.0f};
        float k{10.0f};
        float gravity{-9.81f};
    };
    
    PhysicsData physicsData;
    
private:
    MeshComponent* mesh;
    SpringComponent* spring;
    VectorVisualizerComponent* forceVisualizer;
    
    glm::vec3 velocity{0.0f};
};
