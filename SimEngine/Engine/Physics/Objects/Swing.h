#pragma once

#include "Scene/Objects/Entities/MeshEntity.h"
#include "Core/Plot.h"

class VectorVisualizerComponent;
class SegmentComponent;

class Swing : public MeshEntity
{
public:
    Swing(ObjectBase* parent, Scene* scene, const std::string& name);
    
    void Tick(float deltaTime) override;
    
    void DrawUI() override;
    
    struct PhysicsData
    {
        float swingImpulse{50.0f};
        float seatMass{10.0f};
        float initialLenght{1.5f};
        float gravity{-9.81f};
        float airResistance{10000.0f};
        bool useGravity{true};
        bool useAirResistance{true};
    };
    
    PhysicsData physicsData;
    
private:
    void ApplySwingImpulse();
    
    SegmentComponent* segment;
    MeshComponent* attachPoint;
    MeshComponent* seat;
    VectorVisualizerComponent* velocityVisualizer;
    
    glm::vec3 velocity{0.0f};
    
    const float meshLenght{3.0f};
    const glm::vec3 start{0.0f};
    glm::vec3 end{0.0f, -physicsData.initialLenght, 0.0f};
    
    bool swingImpulseApplied{false};
    
    RuntimePlotData runtimePlotData;
};
