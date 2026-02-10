#include "ProceduralPlanet.h"

#include "Components/ProceduralSphereMeshComponent.h"
#include "Managers/MaterialManager.h"
#include "Managers/MeshManager.h"
#include "Rendering/Renderer/Renderer.h"
#include "Components/PhysicsComponent.h"

ProceduralPlanet::ProceduralPlanet(ObjectBase* parent, Scene* scene, const std::string& name)
    : Entity(parent, scene, name)
{
    meshComponent = AddComponent<ProceduralSphereMeshComponent>();
    
    physicsComponent = AddComponent<PhysicsComponent>();
    
    physicsComponent->physicsData.angularDamping = 1.0f;
    physicsComponent->physicsData.angularVelocity = {0.0f, 10.0f, 0.0f};
    physicsComponent->physicsData.rotateWithCenterOfMass = false;
    physicsComponent->physicsData.canBounce = false;
}

void ProceduralPlanet::SetPlanetColourGradient(const Gradient1D& newGradient)
{
    planetColourGradient = newGradient;
    
    MaterialManager::Get().RegisterCreateAsset("procedural_planet_material" + std::to_string(materialIndex++), [&]
    {
        MaterialResources resources;
        MaterialData& data = resources.data;
        
        data.ambient = {1.0f, 1.0f, 1.0f};
        data.diffuse = {1.0f, 1.0f, 1.0f};
        data.specular ={1.0f, 1.0f, 1.0f};
        data.shininess = 8.0f;  
        
        resources.texture = planetColourGradient.GetTexture();
        return std::make_shared<Material>(resources); 
    });
    
    meshComponent->material = MaterialManager::Get().GetAssetByName("procedural_planet_material" + std::to_string(materialIndex - 1));
}
