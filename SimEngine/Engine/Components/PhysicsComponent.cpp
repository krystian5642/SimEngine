#include "PhysicsComponent.h"

#include "Scene/Systems/Physics/PhysicsSystem.h"
#include "Components/MeshComponent.h"
#include "Scene/Objects/Entities/Entity.h"
#include "Scene/Scene.h"

PhysicsComponent::PhysicsComponent(ObjectBase* parent, Scene* scene, const std::string& name)
    : Component(parent, scene, name)
{
}

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

void PhysicsComponent::Start()
{
    Component::Init();
    CalculatePhysicsData();
}

void PhysicsComponent::Tick(float deltaTime)
{
    Component::Tick(deltaTime);
    
    const auto mask = glm::vec3{1.0f} - glm::vec3(physicsData.physicsLinearConstraints);
    
    // linear velocity
    runtimePhysicsData.linearVelocity *= std::pow(runtimePhysicsData.linearDamping, deltaTime);
    const auto currentAcceleration = physicsData.mass > 0.01f ? runtimePhysicsData.forceAccumulator * mask / physicsData.mass : glm::vec3{0.0f};
    runtimePhysicsData.forceAccumulator = glm::vec3{0.0f};
    
    runtimePhysicsData.linearVelocity += currentAcceleration * deltaTime;
    const auto deltaMove = runtimePhysicsData.linearVelocity * deltaTime;
    runtimePhysicsData.centerOfMass += deltaMove;
    Move(deltaMove);
    
    // angular velocity
    runtimePhysicsData.angularVelocity *= std::pow(runtimePhysicsData.angularDamping, deltaTime);
    const auto angularAcceleration = runtimePhysicsData.momentOfInertia > 0.01f ? runtimePhysicsData.torqueAccumulator / runtimePhysicsData.momentOfInertia : glm::vec3{0.0f};
    runtimePhysicsData.torqueAccumulator = glm::vec3{0.0f};
    
    runtimePhysicsData.angularVelocity += angularAcceleration * deltaTime;
    Rotate({0.0f, 0.0f, runtimePhysicsData.angularVelocity.z * deltaTime}); // for now, only one axis
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
    runtimePhysicsData.forceAccumulator += force;
}

void PhysicsComponent::ApplyTorque(const glm::vec3& force, const glm::vec3& point)
{
    runtimePhysicsData.torqueAccumulator += glm::cross(point - runtimePhysicsData.centerOfMass, force);
    ApplyForce(force);
}

void PhysicsComponent::Move(const glm::vec3& moveDelta)
{
    parentEntity->Move(moveDelta);
}

void PhysicsComponent::Rotate(const glm::vec3& rotationDelta)
{
    parentEntity->Rotate(rotationDelta);
}

bool PhysicsComponent::CollidesWith(const PhysicsComponent* other) const
{
    if (other == this)
    {
        return false;
    }
    
    // only for uniformly scaled spheres
    const float distance = glm::distance(GetPosition(), other->GetPosition());
    const float radius1 = parentEntity->GetScale().x;
    const float radius2 = other->parentEntity->GetScale().x;
    
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

void PhysicsComponent::CalculatePhysicsData()
{
    std::vector<MeshComponent*> meshes;
    parentEntity->GetComponentsByClass<MeshComponent>(meshes);
    const float massDi = physicsData.mass / static_cast<float>(meshes.size());
    
    // center of mass
    for (const auto& mesh : meshes)
    {
        runtimePhysicsData.centerOfMass += massDi * mesh->GetPosition();
    }
    runtimePhysicsData.centerOfMass /= physicsData.mass;
    
    // moment of inertia
    for (const auto& mesh : meshes)
    {
        const auto radius = mesh->GetPosition() - runtimePhysicsData.centerOfMass;
        runtimePhysicsData.momentOfInertia += massDi * glm::dot(radius, radius);
    }
}
