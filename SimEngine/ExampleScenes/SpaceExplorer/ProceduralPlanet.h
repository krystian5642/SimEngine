#pragma once

#include "Scene/Objects/Entities/Entity.h"
#include "Core/Gradient1D.h"

class PhysicsComponent;
class ProceduralSphereMeshComponent;

class ProceduralPlanet : public Entity
{
public:
    ProceduralPlanet(ObjectBase* parent, Scene* scene, const std::string& name);
    
    ProceduralSphereMeshComponent* GetMeshComponent() const { return meshComponent; }
    PhysicsComponent* GetPhysicsComponent() const { return physicsComponent; }
    
    void SetPlanetColourGradient(const Gradient1D& newGradient);
    
private:
    ProceduralSphereMeshComponent* meshComponent;
    PhysicsComponent* physicsComponent;
    
    Gradient1D planetColourGradient;
    
    inline static unsigned int materialIndex = 0;
};
