#pragma once

#include "Component.h"

namespace SimEngine
{
    struct PhysicsData
    {
        float mass{1.0f};
        glm::vec3 velocity{};
        bool enableGravity{false};
    };

    class PhysicsComponent : public Component
    {
    public:
        PhysicsComponent(ObjectBase* parent, Scene* scene, const std::string& name);
        ~PhysicsComponent() override;
    
        static constexpr float gravity{9.81f};
    
        void Tick(float deltaTime) override;
    
        void ApplyForce(const glm::vec3& force);
    
        PhysicsData physicsData;
    
    private:
        glm::vec3 forceAccumulator{};
    };
}
