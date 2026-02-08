#pragma once

#include "Scene/Objects/Entities/MeshEntity.h"
#include "TerrainFace.h"
#include "Gradient1D.h"

class PhysicsComponent;

class ProceduralPlanet : public Entity
{
public:
    ProceduralPlanet(ObjectBase* parent, Scene* scene, const std::string& name);
    
    std::vector<NoiseSettings> noiseSettings;
    
    void GenerateFaces();
    
private:
    std::vector<MeshComponent*> faces;
    
    Gradient1D planetColourGradient;
    
    PhysicsComponent* physicsComponent;
};
