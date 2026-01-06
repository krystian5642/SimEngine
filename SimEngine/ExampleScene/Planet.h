#pragma once

#include "Scene/Objects/Entities/MeshEntity.h"

class GravityComponent;

namespace SimEngine
{
    class LineComponent;
}

class Planet : public SimEngine::MeshEntity
{
public:
    Planet(ObjectBase* parent, SimEngine::Scene* scene, const std::string& name);
    
    void Tick(float deltaTime) override;
    
private:
    GravityComponent* gravityComponent;
    SimEngine::LineComponent* lineComponent;
    
};
