#include "ProceduralPlanet.h"

#include "TerrainFace.h"
#include "Components/PhysicsComponent.h"
#include "Managers/MaterialManager.h"
#include "Rendering/Core/Mesh.h"
#include "Rendering/Renderer/Renderer.h"

ProceduralPlanet::ProceduralPlanet(ObjectBase* parent, Scene* scene, const std::string& name)
    : Entity(parent, scene, name)
{
    /*planetColourGradient.AddPoint(0.0f, glm::vec3(0.0f, 0.0f, 0.2f));
    planetColourGradient.AddPoint(0.4f, glm::vec3(0.0f, 0.3f, 0.6f));
    planetColourGradient.AddPoint(0.6f, glm::vec3(0.1f, 0.5f, 0.2f));
    planetColourGradient.AddPoint(0.8f, glm::vec3(0.5f, 0.4f, 0.2f));
    planetColourGradient.AddPoint(0.95f, glm::vec3(0.9f, 0.9f, 0.9f));
    planetColourGradient.AddPoint(1.0f, glm::vec3(1.0f, 1.0f, 1.0f));*/
    
    planetColourGradient.AddPoint(0.0f,  glm::vec3(0.02f, 0.00f, 0.03f));
    planetColourGradient.AddPoint(0.2f,  glm::vec3(0.10f, 0.00f, 0.20f));
    planetColourGradient.AddPoint(0.35f, glm::vec3(0.05f, 0.25f, 0.00f));
    planetColourGradient.AddPoint(0.55f, glm::vec3(0.30f, 0.80f, 0.10f));
    planetColourGradient.AddPoint(0.75f, glm::vec3(0.80f, 0.90f, 0.05f));
    planetColourGradient.AddPoint(0.9f,  glm::vec3(0.90f, 0.20f, 0.80f));
    planetColourGradient.AddPoint(1.0f,  glm::vec3(1.00f, 1.00f, 0.80f));

    
    MaterialManager::Get().RegisterCreateAsset("procedural_planet_material", [&]
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
    
    faces.reserve(6);
    
    for (int i = 0; i < 6; i++)
    {
        auto meshComponent = AddComponent<MeshComponent>();
        meshComponent->material = MaterialManager::Get().GetAssetByName("procedural_planet_material");
        faces.push_back(meshComponent);
    }
    
    GenerateFaces();
    
    physicsComponent = AddComponent<PhysicsComponent>();
    physicsComponent->physicsData.angularDamping = 1.0f;
    physicsComponent->physicsData.angularVelocity = {0.0f, 100.0f, 0.0f};
    physicsComponent->physicsData.rotateWithCenterOfMass = false;
}

void ProceduralPlanet::GenerateFaces()
{
    const glm::vec3 UP    = glm::vec3(0.0f, 1.0f, 0.0f);
    const glm::vec3 DOWN  = glm::vec3(0.0f, -1.0f, 0.0f);
    const glm::vec3 RIGHT = glm::vec3(1.0f, 0.0f, 0.0f);
    const glm::vec3 LEFT  = glm::vec3(-1.0f, 0.0f, 0.0f);
    const glm::vec3 FORWARD = glm::vec3(0.0f, 0.0f, -1.0f);
    const glm::vec3 BACK    = glm::vec3(0.0f, 0.0f, 1.0f);
    
    std::vector directions = {
        UP,
        DOWN,
        LEFT,
        RIGHT,
        FORWARD,
        BACK
    };
    
    float minHeight{INFINITY};
    float maxHeight{-INFINITY};
    
    std::vector<MeshData> meshDatas;
    for (int i = 0; i < 6; i++)
    {
        const auto& direction = directions[i];
        MeshData faceData;
        TerrainFace::GenerateMeshData(direction, 30, noiseSettings, faceData, minHeight, maxHeight);
        
        meshDatas.push_back(faceData);
    }
    
    for (auto& meshData : meshDatas)
    {
        std::vector<float>& vertices = meshData.vertices;
        
        for (int i = 0; i < vertices.size(); i += 8)
        {
            glm::vec3 vertex = glm::vec3{vertices[i], vertices[i + 1], vertices[i + 2]};
            float length = glm::length(vertex);
        
            float& u = vertices[i + 3];
        
            u = (length - minHeight) / (maxHeight - minHeight);
        }
    }
    
    for (int i = 0; i < 6; i++)
    {
        auto& face = meshDatas[i];
        faces[i]->mesh = Renderer::CreateMeshStatic(face);
    }
}
