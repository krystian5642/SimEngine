#pragma once

#include "Components/Component.h"

class Entity;
class GravitySystem;

struct GravityData
{
    float mass{1.0f};
    glm::vec3 velocity{};
    glm::vec3 currentGravityForce{};
};

class GravityComponent : public Component
{
public:
    GravityComponent(ObjectBase* parent, Scene* scene, const std::string& name);
  
    void Init() override;
    void Tick(float deltaTime) override;
    void OnDestroy() override;

    const glm::vec3& GetPosition() const;

    GravityData gravityData;
    bool showTrajectory{false};
    glm::bvec3 gravityConstraints{false};

private:
    std::vector<glm::vec3> trajectory;
    static constexpr size_t trajectoryLength{100000};

    Entity* parentEntity{};
    GravitySystem* gravitySystem{};
};