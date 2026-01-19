#pragma once

#include "Scene/Objects/Entities/Entity.h"

class MeshEntity;

class PlanetSystem : public Entity
{
public:
    PlanetSystem(ObjectBase* parent, Scene* scene, const std::string& name);

    void Tick(float deltaTime) override;

    private:
    MeshEntity* planet;
};
