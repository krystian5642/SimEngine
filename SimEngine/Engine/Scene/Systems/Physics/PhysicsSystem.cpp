#include "PhysicsSystem.h"

PhysicsSystem::PhysicsSystem(ObjectBase* parent, Scene* scene, const std::string& name)
    : SceneObject(parent, scene, name)
{
}

void PhysicsSystem::Tick(float deltaTime)
{
    SceneObject::Tick(deltaTime);
    
    for (size_t i = 0; i < physicsComponents.size(); i++)
    {
        auto* phys1 = physicsComponents[i];
        for (size_t j = i + 1; j < physicsComponents.size(); j++)
        {
            auto* phys2 = physicsComponents[j];
            
            // only for uniformly scaled spheres
            if (phys1->CollidesWith(phys2))
            {
                const glm::vec3 n = glm::normalize(phys2->GetPosition() - phys1->GetPosition());
            
                const glm::vec3 V1n = glm::dot(phys1->physicsData.velocity, n) * n;
                const glm::vec3 V2n = glm::dot(phys2->physicsData.velocity, n) * n;
            
                const glm::vec3 V1p = phys1->physicsData.velocity - V1n;
                const glm::vec3 V2p = phys2->physicsData.velocity - V2n;
            
                const glm::vec3 V1primN = (V1n * (phys1->physicsData.mass - phys2->physicsData.mass) + 2.0f * phys2->physicsData.mass * V2n) / (phys1->physicsData.mass + phys2->physicsData.mass);
                const glm::vec3 V2primN = (V2n * (phys2->physicsData.mass - phys1->physicsData.mass) + 2.0f * phys1->physicsData.mass * V1n) / (phys1->physicsData.mass + phys2->physicsData.mass);
            
                phys1->physicsData.velocity = V1primN + V1p;
                phys2->physicsData.velocity = V2primN + V2p;
                
                phys1->Move(-n * 0.02f);
                phys2->Move(n * 0.02f);
            }
        }
    }
}
    
void PhysicsSystem::RegisterPhysicsComponent(PhysicsComponent* component)
{
    physicsComponents.push_back(component);
}
    
void PhysicsSystem::UnregisterPhysicsComponent(PhysicsComponent* component)
{
    std::erase(physicsComponents, component);
}