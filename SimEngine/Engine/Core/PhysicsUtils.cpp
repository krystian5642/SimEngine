#include "PhysicsUtils.h"

void PhysicsUtils::PredictProjectileTrajectory(const ProjectileTrajectoryData& trajectoryData, std::vector<glm::vec3>& outTrajectory)
{
    outTrajectory.clear();
    outTrajectory.reserve(static_cast<size_t>(trajectoryData.time / trajectoryData.timeStep) + 1);
    
    for (float t = 0.0f; t <= trajectoryData.time; t += trajectoryData.timeStep)
    {
        glm::vec3 point;
        point.x = trajectoryData.startPosition.x + trajectoryData.velocity.x * t;
        point.y = trajectoryData.startPosition.y + trajectoryData.velocity.y * t - 0.5f * trajectoryData.gravity * t * t;
        point.z = trajectoryData.startPosition.z + trajectoryData.velocity.z * t;
    
        outTrajectory.push_back(point);
    
        if (point.y <= trajectoryData.groundLevel) break;
    }
}
