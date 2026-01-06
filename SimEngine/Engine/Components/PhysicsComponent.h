#pragma once

#include "Component.h"

namespace SimEngine
{
    class Entity;

    struct PhysicsData
    {
        float mass{1.0f};
        glm::vec3 velocity{};
        bool enableGravity{false};
        bool applyFriction{true};
    };
    
    struct ScenePhysicsConstants
    {
        static constexpr float gravity{9.81f};
        static constexpr float groundLevel{-2.0f};
    };

    class PhysicsComponent : public Component
    {
    public:
        PhysicsComponent(ObjectBase* parent, Scene* scene, const std::string& name);
        ~PhysicsComponent() override;
        
        void Init() override;
        void Tick(float deltaTime) override;
    
        void ApplyForce(const glm::vec3& force);
    
        PhysicsData physicsData;
    
    private:
        glm::vec3 forceAccumulator{};
        
        Entity* parentEntity{};
    };
}
