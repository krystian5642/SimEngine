#include "PhysicsComponent.h"

#include "Scene/Systems/Physics/PhysicsSystem.h"
#include "Components/MeshComponent.h"
#include "Scene/Objects/Entities/Entity.h"
#include "Scene/Scene.h"

PhysicsComponent::PhysicsComponent(ObjectBase* parent, Scene* scene, const std::string& name)
    : Component(parent, scene, name)
{
   tickPhase = EngineTickPhase::Physics;
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
    
    // linear velocity
    physicsData.linearVelocity *= std::pow(physicsData.linearDamping, deltaTime);
    physicsData.linearVelocity += physicsData.linearAcceleration * deltaTime;
    physicsData.linearAcceleration = glm::vec3{0.0f};
    
    const auto deltaMove = physicsData.linearVelocity * deltaTime;
    physicsData.centerOfMass += deltaMove;
    
    Move(deltaMove);
    
    // angular velocity
    physicsData.angularVelocity *= std::pow(physicsData.angularDamping, deltaTime);
    physicsData.angularVelocity += physicsData.angularAcceleration * deltaTime;
    physicsData.angularAcceleration = glm::vec3{0.0f};
    physicsData.angularAcceleration = glm::vec3{0.0f};
    
    ApplyAngularVelocity(deltaTime);
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
    const auto mask = glm::vec3{1.0f} - glm::vec3(physicsData.physicsLinearConstraints);
    physicsData.linearAcceleration += physicsData.mass > 0.01f ? force * mask / physicsData.mass : glm::vec3{0.0f};
}

void PhysicsComponent::ApplyTorque(const glm::vec3& force, const glm::vec3& point)
{
    const auto torque = glm::cross(point - physicsData.centerOfMass, force);
    physicsData.angularAcceleration += physicsData.invertedInertiaTensor * torque;
    
    ApplyForce(force);
}

void PhysicsComponent::ApplyImpulse(const glm::vec3& impulse)
{
    physicsData.linearVelocity += impulse / physicsData.mass;
}

void PhysicsComponent::ApplyAngularImpulse(const glm::vec3& impulse)
{
    physicsData.angularVelocity += impulse * physicsData.invertedInertiaTensor;
}

void PhysicsComponent::Move(const glm::vec3& moveDelta)
{
    parentEntity->Move(moveDelta);
}

void PhysicsComponent::StopImediately()
{
    physicsData.linearVelocity = glm::vec3{0.0f};
    physicsData.angularVelocity = glm::vec3{0.0f};
}

bool PhysicsComponent::CollidesWith(const PhysicsComponent* other) const
{
    if (other == this || other == nullptr 
        || !other->physicsData.enableCollision || !physicsData.enableCollision)
    {
        return false;
    }
    
    // only for uniformly scaled spheres
    const float distance = glm::distance(GetPosition(), other->GetPosition());
    const float radius1 = parentEntity->GetScale().x * radiusMultiplier;
    const float radius2 = other->parentEntity->GetScale().x * other->radiusMultiplier;
    
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
    physicsData.centerOfMass = glm::vec3{0.0f, 0.0f, 0.0f};
    physicsData.invertedInertiaTensor = glm::mat3{1.0f};
    
    std::vector<MeshComponent*> meshes;
    parentEntity->GetComponentsByClass<MeshComponent>(meshes);
    
    if (!meshes.empty())
    {
        const auto massDi = physicsData.mass / static_cast<float>(meshes.size());
    
        // center of mass
        for (const auto& mesh : meshes)
        {
            physicsData.centerOfMass += massDi * mesh->GetPosition();
        }
        physicsData.centerOfMass /= physicsData.mass;
    
        float Ixx = 0.0f;
        float Iyy = 0.0f;
        float Izz = 0.0f;
        
        // inverted inertia tensor
        for (const auto& mesh : meshes)
        {
            const auto& position = mesh->GetPosition() - physicsData.centerOfMass;
            
            const auto x2 = position.x * position.x;
            const auto y2 = position.y * position.y;
            const auto z2 = position.z * position.z;
            
            Ixx += y2 + z2;
            Iyy += x2 + z2;
            Izz += x2 + y2;
        }
        
        physicsData.invertedInertiaTensor = glm::mat3{
            Ixx, 0.0f, 0.0f
            , 0.0f, Iyy, 0.0f
            , 0.0f, 0.0f, Izz};
        
        const auto det = glm::determinant(physicsData.invertedInertiaTensor);
        if (std::abs(det) > 1e-6f) 
        {
            physicsData.invertedInertiaTensor = glm::inverse(physicsData.invertedInertiaTensor);
        }
    }
}

void PhysicsComponent::ApplyAngularVelocity(float deltaTime)
{
    if (physicsData.rotateWithCenterOfMass)
    {
        std::vector<MeshComponent*> meshes;
        parentEntity->GetComponentsByClass<MeshComponent>(meshes);
    
        const auto angularSpeed = glm::length(physicsData.angularVelocity);
        if (angularSpeed > 1e-6f)
        {
            const auto deltaAngle = angularSpeed * deltaTime;
            const auto axis = physicsData.angularVelocity / angularSpeed;
        
            const auto quatRotation = glm::angleAxis(deltaAngle, axis);
            for (const auto& mesh : meshes)
            {
                const auto relativePosition = quatRotation * (mesh->GetPosition() - physicsData.centerOfMass);
                mesh->SetPosition(physicsData.centerOfMass + relativePosition);
            }
        }
    }
    else
    {
        const auto deltaRotation = physicsData.angularVelocity * deltaTime;
        parentEntity->Rotate(deltaRotation);
    }
}
