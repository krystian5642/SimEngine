#pragma once

#include "Scene/Objects/Entities/MeshEntity.h"

class GravityComponent;

class LineComponent;

class Planet : public MeshEntity
{
public:
    Planet(ObjectBase* parent, Scene* scene, const std::string& name);

    void Tick(float deltaTime) override;

private:
    GravityComponent* gravityComponent;
    LineComponent* lineComponent;

};
