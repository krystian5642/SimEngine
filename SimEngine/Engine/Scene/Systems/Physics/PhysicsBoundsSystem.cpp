#include "PhysicsBoundsSystem.h"

#include "Components/PhysicsComponent.h"

PhysicsBoundsSystem::PhysicsBoundsSystem(ObjectBase* parent, Scene* scene, const std::string& name)
    : PhysicsSystem(parent, scene, name)
{
}

void PhysicsBoundsSystem::Tick(float deltaTime)
{
    PhysicsSystem::Tick(deltaTime);
    
    for (const auto& physicsComponent : physicsComponents)
    {
        if (physicsComponent->physicsData.useBounds)
        {
            const auto position = physicsComponent->GetPosition();
            const auto radius = physicsComponent->GetRadius();
    
            glm::vec3 normal(0.0f);

            // X MIN
            if (position.x - radius < boundingBox.minBounds.x)
            {
                normal = glm::vec3(1,0,0);
            }
            // X MAX
            else if (position.x + radius > boundingBox.maxBounds.x)
            {
                normal = glm::vec3(-1,0,0);
            }
            // Y MIN
            else if (position.y - radius < boundingBox.minBounds.y)
            {
                normal = glm::vec3(0,1,0);
            }
            // Y MAX
            else if (position.y + radius > boundingBox.maxBounds.y)
            {
                normal = glm::vec3(0,-1,0);
            }
            // Z MIN
            else if (position.z - radius < boundingBox.minBounds.z)
            {
                normal = glm::vec3(0,0,1);;
            }
            // Z MAX
            else if (position.z + radius > boundingBox.maxBounds.z)
            {
                normal = glm::vec3(0,0,-1);
            }
    
            physicsComponent->Move(normal * 0.01f);
    
            auto& linearVelocity = physicsComponent->runtimePhysicsData.linearVelocity;
            linearVelocity = glm::reflect(linearVelocity, normal);
        }
    }
}
