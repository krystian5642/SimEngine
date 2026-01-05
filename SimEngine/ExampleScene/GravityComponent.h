#pragma once

#include "Components/Component.h"

namespace SimEngine
{
    class Entity;
}

class GravitySystem;

struct GravityData
{
    float mass{1.0f};
    glm::vec3 velocity{};
    glm::vec3 currentGravityForce{};
};

class GravityComponent : public SimEngine::Component
{
public:
    GravityComponent(ObjectBase* parent, SimEngine::Scene* scene, const std::string& name);
  
    void Init() override;
    void Tick(float deltaTime) override;
    void OnDestroy() override;
    
    const glm::vec3& GetPosition() const;
    
    GravityData gravityData;
    bool showTrajectory = false;

private:
    std::vector<glm::vec3> trajectory;
    static constexpr size_t trajectoryLength = 100000;

    SimEngine::Entity* parentEntity{};
    GravitySystem* gravitySystem{};
};