#include "PhysicsCollisionSystem.h"

#include "Components/PhysicsComponent.h"

PhysicsCollisionSystem::PhysicsCollisionSystem(ObjectBase* parent, Scene* scene, const std::string& name)
    : PhysicsSystem(parent, scene, name)
{
}

void PhysicsCollisionSystem::Tick(float deltaTime)
{
    SceneObject::Tick(deltaTime);
    
    for (size_t i = 0; i < physicsComponents.size(); i++)
    {
        const auto phys1 = physicsComponents[i];
        for (size_t j = i + 1; j < physicsComponents.size(); j++)
        {
            const auto phys2 = physicsComponents[j];
            
            // only for uniformly scaled spheres
            if (phys1->CollidesWith(phys2))
            {
                const auto n = glm::normalize(phys2->GetPosition() - phys1->GetPosition());
                
                const auto V1n = glm::dot(phys1->physicsData.linearVelocity , n) * n;
                const auto V2n = glm::dot(phys2->physicsData.linearVelocity, n) * n;
                
                if (phys1->physicsData.canBounce)
                {
                    const auto V1p = phys1->physicsData.linearVelocity - V1n;
                    const auto V1primN = (V1n * (phys1->physicsData.mass - phys2->physicsData.mass) + 2.0f * phys2->physicsData.mass * V2n) / (phys1->physicsData.mass + phys2->physicsData.mass);
                    
                    phys1->physicsData.linearVelocity = V1primN + V1p;
                }
                else if (phys2->physicsData.stopAtCollision)
                {
                    phys2->StopImediately();
                }
                
                if (phys2->physicsData.canBounce)
                {
                    const auto V2p = phys2->physicsData.linearVelocity - V2n;
                    const auto V2primN = (V2n * (phys2->physicsData.mass - phys1->physicsData.mass) + 2.0f * phys1->physicsData.mass * V1n) / (phys1->physicsData.mass + phys2->physicsData.mass);
                    
                    phys2->physicsData.linearVelocity = V2primN + V2p;
                }
                else if (phys1->physicsData.stopAtCollision)
                {
                    phys1->StopImediately();
                }
                
                phys1->Move(-n * 0.02f);
                phys2->Move(n * 0.02f);
            }
        }
    }
}