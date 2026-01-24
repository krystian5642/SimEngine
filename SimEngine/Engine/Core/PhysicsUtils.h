#pragma once

struct ProjectileTrajectoryData
{
    glm::vec3 startPosition{0.0f};
    glm::vec3 velocity{0.0f};
    float linearDamping{0.999f};
    float time{1.0f};
    float timeStep{0.1f};
    float gravity{9.81f};
    float groundLevel{-2.5f};
};

class PhysicsUtils
{
public:
    static void PredictProjectileTrajectory(const ProjectileTrajectoryData& trajectoryData, std::vector<glm::vec3>& outTrajectory);
};
