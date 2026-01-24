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
    
    CalculatePhysicsData(); // temp !!!
    
    const auto mask = glm::vec3{1.0f} - glm::vec3(physicsData.physicsLinearConstraints);
    
    // linear velocity
    physicsData.linearVelocity *= std::pow(physicsData.linearDamping, deltaTime);
    const auto currentAcceleration = physicsData.mass > 0.01f ? physicsData.forceAccumulator * mask / physicsData.mass : glm::vec3{0.0f};
    physicsData.forceAccumulator = glm::vec3{0.0f};
    
    physicsData.linearVelocity += currentAcceleration * deltaTime;
    const auto deltaMove = physicsData.linearVelocity * deltaTime;
    //physicsData.centerOfMass += deltaMove;
    Move(deltaMove);
    
    // angular velocity
    physicsData.angularVelocity *= std::pow(physicsData.angularDamping, deltaTime);
    const auto angularAcceleration = physicsData.momentOfInertia > 0.01f ? physicsData.torqueAccumulator / physicsData.momentOfInertia : glm::vec3{0.0f};
    physicsData.torqueAccumulator = glm::vec3{0.0f};
    
    physicsData.angularVelocity += angularAcceleration * deltaTime;
    Rotate({0.0f, physicsData.angularVelocity.y * deltaTime, physicsData.angularVelocity.z * deltaTime}); // for now, only one axis
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
    physicsData.forceAccumulator += force;
}

void PhysicsComponent::ApplyTorque(const glm::vec3& force, const glm::vec3& point)
{
    physicsData.torqueAccumulator += glm::cross(point - physicsData.centerOfMass, force);
    ApplyForce(force);
}

void PhysicsComponent::Move(const glm::vec3& moveDelta)
{
    parentEntity->Move(moveDelta);
}

void PhysicsComponent::Rotate(const glm::vec3& rotationDelta)
{
    if (physicsData.rotateWithCenterOfMass)
    {
        std::vector<MeshComponent*> meshes;
        parentEntity->GetComponentsByClass<MeshComponent>(meshes);
    
        for (const auto& mesh : meshes)
        {
            const glm::mat4 rotationMatrix = glm::rotate(glm::mat4{1.0f}, rotationDelta.z, glm::vec3{0.0f, 0.0f, 1.0f});
            const glm::vec3 relativePosition = rotationMatrix * glm::vec4(mesh->GetPosition() - physicsData.centerOfMass, 1.0f);
            mesh->SetPosition(physicsData.centerOfMass + relativePosition);
        }
    }
    else
    {
        parentEntity->Rotate(rotationDelta);
    }
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
    physicsData.centerOfMass = glm::vec3{0.0f, 0.0f, 0.0f}; //temp!!
    physicsData.momentOfInertia = 0.0f; // temp!!
    
    std::vector<MeshComponent*> meshes;
    parentEntity->GetComponentsByClass<MeshComponent>(meshes);
    
    if (!meshes.empty())
    {
        const float massDi = physicsData.mass / static_cast<float>(meshes.size());
    
        // center of mass
        for (const auto& mesh : meshes)
        {
            physicsData.centerOfMass += massDi * mesh->GetPosition();
        }
        physicsData.centerOfMass /= physicsData.mass;
    
        // moment of inertia
        for (const auto& mesh : meshes)
        {
            const auto radius = mesh->GetPosition() - physicsData.centerOfMass;
            physicsData.momentOfInertia += massDi * glm::dot(radius, radius);
        }
    }
}
