#include "PhysicsUtils.h"

void PhysicsUtils::PredictProjectileTrajectory(const ProjectileTrajectoryData& trajectoryData, std::vector<glm::vec3>& outTrajectory)
{
    outTrajectory.clear();
    outTrajectory.reserve(static_cast<size_t>(trajectoryData.time / trajectoryData.timeStep) + 1);
    
    auto velocity = trajectoryData.velocity;
    auto prevPoint = trajectoryData.startPosition;
    for (float t = 0.0f; t <= trajectoryData.time; t += trajectoryData.timeStep)
    {
        const auto newPoint = prevPoint + velocity * trajectoryData.timeStep;
        outTrajectory.push_back(newPoint);
    
        velocity *= std::pow(trajectoryData.linearDamping, trajectoryData.timeStep);
        velocity.y -= trajectoryData.gravity * trajectoryData.timeStep;
        prevPoint = newPoint;
        
        if (newPoint.y <= trajectoryData.groundLevel) break;
    }
}
