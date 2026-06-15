#include "PhysicsComponent.h"

#include "PhysicsSystem.h"
#include "Scene/Objects/Entities/Entity.h"
#include "Scene/Scene.h"

void PhysicsComponent::Init()
{
    Component::Init();
    
    parentEntity = dynamic_cast<Entity*>(parent);
    
    scene->GetObjectsByClass<PhysicsSystem>(scenePhysicsSystems);
    for (const auto physicsSystem : scenePhysicsSystems)
    {
        physicsSystem->RegisterPhysicsComponent(this);
    }
}

void PhysicsComponent::Tick(float deltaTime)
{
    Component::Tick(deltaTime);
    
    physicsData.linearAcceleration += physicsData.accumulatedForce / physicsData.mass;
    physicsData.accumulatedForce = glm::vec3{0.0f};
        
    // linear velocity
    if (physicsData.linearDamping > 0.0f)
    {
        physicsData.linearVelocity *= std::pow(physicsData.linearDamping, deltaTime);
    }
    
    physicsData.linearVelocity += physicsData.linearAcceleration * deltaTime;
    physicsData.linearAcceleration = glm::vec3{0.0f};
    
    const auto deltaMove = physicsData.linearVelocity * deltaTime;
    Move(deltaMove);
}

void PhysicsComponent::OnDestroy()
{
    Component::OnDestroy();
    
    for (const auto physicsSystem : scenePhysicsSystems)
    {
        physicsSystem->UnregisterPhysicsComponent(this);
    }
}

void PhysicsComponent::ApplyForce(const glm::vec3& force)
{
    physicsData.accumulatedForce += force;
}

void PhysicsComponent::Move(const glm::vec3& moveDelta)
{
    parentEntity->Move(moveDelta);
}

void PhysicsComponent::StopImediately()
{
    physicsData.linearVelocity = glm::vec3{0.0f};
}

bool PhysicsComponent::CollidesWith(const PhysicsComponent* other) const
{
    if (other == this || other == nullptr)
    {
        return false;
    }
    
    // only for uniformly scaled spheres
    const float distance = glm::distance(GetPosition(), other->GetPosition());
    const float radius1 = GetRadius();
    const float radius2 = other->GetRadius();
    
    if (distance <= radius1 + radius2)
    {
        return true;
    }
    
    return false;
}

float PhysicsComponent::GetRadius() const
{
    return parentEntity->GetScale().x;
}

const glm::vec3& PhysicsComponent::GetPosition() const
{
    return parentEntity->GetPosition();
}