#include "GravitySystem.h"
#include "GravityComponent.h"

GravitySystem::GravitySystem(ObjectBase* parent, SimEngine::Scene* scene, const std::string& name)
    : SceneObject(parent, scene, name)
{
}

void GravitySystem::Tick(float deltaTime)
{
    for (size_t i = 0; i < gravityComponents.size(); i++)
    {
        auto* grav1 = gravityComponents[i];
        for (size_t j = i + 1; j < gravityComponents.size(); j++)
        {
            auto* grav2 = gravityComponents[j];
            
            const glm::vec3 direction = grav2->GetPosition() - grav1->GetPosition();
            
            const float distanceSquared = glm::dot(direction, direction);
            if (distanceSquared < 0.01f) continue;
            
            const float distance = sqrt(distanceSquared);
            const glm::vec3 directionNormalized = direction / distance;
            
            float forceMagnitude = G * grav1->gravityData.mass * grav2->gravityData.mass / distanceSquared;
            
            const glm::vec3 force = forceMagnitude * directionNormalized;
            
            grav1->gravityData.currentGravityForce += force;
            grav2->gravityData.currentGravityForce -= force;
        }
    }
}

void GravitySystem::RegisterGravityComponent(GravityComponent* gravityComponent)
{
    gravityComponents.push_back(gravityComponent);
}

void GravitySystem::UnregisterGravityComponent(GravityComponent* gravityComponent)
{
    std::erase(gravityComponents, gravityComponent);
}
