
#include "PhysicsComponent.h"

#include "Scene/Objects/Entities/MeshEntity.h"

namespace SimEngine
{
    PhysicsComponent::PhysicsComponent(ObjectBase* parent, Scene* scene, const std::string& name)
        : Component(parent, scene, name)
    {
    }

    PhysicsComponent::~PhysicsComponent()
    {
    }

    void PhysicsComponent::Init()
    {
        Component::Init();
        
        parentEntity = dynamic_cast<Entity*>(parent);
    }

    void PhysicsComponent::Tick(float deltaTime)
    {
        if (parentEntity->GetPosition().y <= ScenePhysicsConstants::groundLevel)
        {
            return;
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

    void PhysicsComponent::ApplyForce(const glm::vec3& force)
    {
        forceAccumulator += force;
    }
}
