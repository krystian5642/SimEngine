#pragma once

#include "Scene/Objects/Entities/Entity.h"

namespace SimEngine
{
    class MeshEntity;
}

class PlanetSystem : public SimEngine::Entity
{
public:
    PlanetSystem(ObjectBase* parent, SimEngine::Scene* scene, const std::string& name);
    
    void Tick(float deltaTime) override;
    
private:
    SimEngine::MeshEntity* planet;
};
