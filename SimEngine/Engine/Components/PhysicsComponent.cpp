
#include "PhysicsComponent.h"

#include "Scene/Scene.h"
#include "Scene/Objects/Entities/MeshEntity.h"
#include "Scene/Systems/Physics/PhysicsSystem.h"

PhysicsComponent::PhysicsComponent(ObjectBase* parent, Scene* scene, const std::string& name)
    : Component(parent, scene, name)
{
}

void PhysicsComponent::Init()
{
    Component::Init();
    
    parentEntity = dynamic_cast<Entity*>(parent);
    physicsSystem = scene->GetObjectByClass<PhysicsSystem>();
    physicsSystem->RegisterPhysicsComponent(this);
}
    
void PhysicsComponent::Tick(float deltaTime)
{
    if (physicsData.useBounds)
    {
        const glm::vec3 position = GetPosition();
        const float radius = parentEntity->GetScale().x;
    
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
    
        Move(normal * 0.01f);
    
        physicsData.velocity = glm::reflect(physicsData.velocity, normal);
    }
    
    const glm::vec2 horizontalVelPrev = glm::vec2{physicsData.velocity.x, physicsData.velocity.z};
    const float horizontalSpeedPrev = glm::length(horizontalVelPrev);
    const float prevPitch = horizontalSpeedPrev > 0.0f ? glm::degrees(glm::atan(physicsData.velocity.y / horizontalSpeedPrev)) : 0.0f;

    if (physicsData.applyFriction)
    {
        // Friction
        const float speed = glm::length(physicsData.velocity);
        if (speed > 0.01f)
        {
            const float coefficientOfFriction = speed > 0.1f ? 0.5f : 0.7f;
            const glm::vec3 frictionalForce = -physicsData.mass * ScenePhysicsConstants::gravity * coefficientOfFriction * physicsData.velocity / speed;
            ApplyForce(frictionalForce);
        }
    }
    else
    {
        physicsData.velocity *= physicsData.linearDamping;
    }
    
    glm::vec3 currentAcceleration = forceAccumulator / physicsData.mass;
    if (physicsData.enableGravity)
    {
        currentAcceleration.y -= ScenePhysicsConstants::gravity;
    }
    forceAccumulator = glm::vec3{0.0f};
    
    physicsData.velocity += currentAcceleration * deltaTime;
    const glm::vec3 deltaMove = physicsData.velocity * deltaTime;
    parentEntity->Move(deltaMove);
    
    const glm::vec2 horizontalVelCurr = glm::vec2{physicsData.velocity.x, physicsData.velocity.z};
    const float horizontalSpeedCurr = glm::length(horizontalVelCurr);
    const float currPitch = horizontalSpeedCurr > 0.0f ? glm::degrees(glm::atan(physicsData.velocity.y / horizontalSpeedCurr)) : 0.0f;

    parentEntity->Rotate({currPitch - prevPitch, 0.0f, 0.0f});
}
    
void PhysicsComponent::OnDestroy()
{
    Component::OnDestroy();
    
    physicsSystem->UnregisterPhysicsComponent(this);
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
    
void PhysicsComponent::ApplyForce(const glm::vec3& force)
{
    forceAccumulator += force;
}
    
const glm::vec3& PhysicsComponent::GetPosition() const
{
    return parentEntity->GetPosition();
}
    
void PhysicsComponent::Move(const glm::vec3& moveDelta)
{
    parentEntity->Move(moveDelta);
}