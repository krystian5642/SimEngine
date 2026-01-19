#include "ExampleApp.h"

#include "BallLauncher.h"
#include "GravityComponent.h"
#include "GravitySystem.h"
#include "PlanetSystem.h"
#include "Core/MathUtils.h"
#include "Managers/MaterialManager.h"
#include "Managers/MeshManager.h"
#include "Managers/TextureManager.h"
#include "Rendering/Core/Material.h"
#include "Rendering/Renderer/Renderer.h"
#include "Scene/Scene.h"
#include "Scene/SceneManager.h"
#include "Scene/Objects/Entities/CameraEntity.h"
#include "Scene/Objects/Lighting/DirectionalLightObject.h"
#include "Rendering/Core/Texture.h"
#include "Scene/Systems/Physics/PhysicsSystem.h"

ExampleApp::ExampleApp()
{
    // jupiter
    TextureManager::Get().RegisterCreateAsset("jupiter", []
    {
        auto texture = Renderer::CreateTextureStatic("Textures/8k_jupiter.jpg");
        texture->Load();
        return texture;
    });

    MaterialManager::Get().RegisterCreateAsset("jupiter", []
    {
        MaterialResources resources;
        MaterialData& data = resources.data;
    
        data.ambient = {0.3f, 0.25f, 0.2f};
        data.specular = {0.1f, 0.1f, 0.1f};
        data.shininess = 0.05f * 128.0f;
    
        resources.texture = TextureManager::Get().GetAssetByName("jupiter");
        return std::make_shared<Material>(resources); 
    });

    // moon
    TextureManager::Get().RegisterCreateAsset("moon", []
    {
        auto texture = Renderer::CreateTextureStatic("Textures/moon.jpg");
        texture->Load();
        return texture;
    });

    MaterialManager::Get().RegisterCreateAsset("moon", []
    {
        MaterialResources resources;
        MaterialData& data = resources.data;
    
        data.ambient = {0.3f, 0.25f, 0.2f};
        data.specular = {0.1f, 0.1f, 0.1f};
        data.shininess = 0.05f * 128.0f;
    
        resources.texture = TextureManager::Get().GetAssetByName("moon");
        return std::make_shared<Material>(resources); 
    });

    MaterialManager::Get().RegisterCreateAsset("reflect", []
    {
        return Renderer::CreateReflectMaterialStatic();
    });

    MaterialManager::Get().RegisterCreateAsset("refract", []
    {
        return Renderer::CreateRefractMaterialStatic();
    });

    auto defaultScene = []
    {
        const std::vector<std::string> skyboxFaces = {
            "Textures/Skybox/cupertin-lake_rt.tga",
            "Textures/Skybox/cupertin-lake_lf.tga",
            "Textures/Skybox/cupertin-lake_up.tga",
            "Textures/Skybox/cupertin-lake_dn.tga",
            "Textures/Skybox/cupertin-lake_bk.tga",
            "Textures/Skybox/cupertin-lake_ft.tga",
        };
    
        auto scene = std::make_unique<Scene>();
        scene->SetSkybox(Renderer::CreateSkyboxStatic(skyboxFaces));
    
        scene->AddObject<BallLauncher>();
    
        auto camera = scene->AddObject<CameraEntity>("Camera");
        camera->SetAsActiveCamera();
        
        scene->AddObject<GravitySystem>();
        scene->AddObject<PlanetSystem>();
    
        auto plane = scene->AddObject<MeshEntity>();
        plane->SetMesh(MeshManager::Get().GetAssetByName("plane"));
        plane->SetMaterial(MaterialManager::Get().GetAssetByName("chrome"));
        plane->SetScale({100.0f, 1.0f, 100.0f});
        plane->Move({0.0f, -2.5f, 0.0f});
    
        auto light = scene->AddObject<DirectionalLightObject>("Directional Light");
        light->SetDirection({0.1f, -60.0f, 0.1f});
        light->lightData.ambient = 0.5f;
        light->lightData.diffuse = 0.8f;
    
        auto ball1 = scene->AddObject<MeshEntity>();
        ball1->SetMesh(MeshManager::Get().GetAssetByName("sphere"));
        ball1->SetMaterial(MaterialManager::Get().GetAssetByName("reflect"));
        ball1->Move({4.0f, 2.0f, 4.0f});
    
        auto ball2 = scene->AddObject<MeshEntity>();
        ball2->SetMesh(MeshManager::Get().GetAssetByName("sphere"));
        ball2->SetMaterial(MaterialManager::Get().GetAssetByName("refract"));
        ball2->Move({-4.0f, 2.0f, 4.0f});
    
        return scene;
    };

    auto scene2 = []
    {
        auto scene = std::make_unique<Scene>();
    
        auto camera = scene->AddObject<CameraEntity>("Camera");
        camera->SetAsActiveCamera();
        
        scene->AddObject<PhysicsSystem>();
        scene->AddObject<GravitySystem>();
    
        auto light = scene->AddObject<DirectionalLightObject>("Directional Light");
        light->SetDirection({0.1f, 0.1f, -60.0f});
        light->lightData.ambient = 1.0f;
    
        /*auto ball2 = scene->AddObject<MeshEntity>();
        auto grav = ball2->AddComponent<GravityComponent>();
        auto phys = ball2->AddComponent<PhysicsComponent>();
        phys->physicsData.enableGravity = false;
        phys->physicsData.applyFriction = false;
        phys->physicsData.velocity = {0.0f, 0.0f, 0.0f};
        phys->physicsData.mass = 10000.0f;
        grav->gravityData.mass = 10.0f;
        ball2->SetMesh(MeshManager::Get().GetAssetByName("sphere"));
        ball2->SetMaterial(MaterialManager::Get().GetAssetByName("chrome"));
        ball2->SetScale({0.1f, 0.1f, 0.1f});*/
    
        for (int i = 0; i < 10; i++)
        {
            for (int j = 0; j <10; j++)
            {
                auto ball = scene->AddObject<MeshEntity>();
                auto phys = ball->AddComponent<PhysicsComponent>();
           
                const float mass = MathUtils::randomNum(0.5f, 1.7f);
                
                phys->physicsData.mass = mass;
                phys->physicsData.enableGravity = false;
                phys->physicsData.applyFriction = false;
                
                constexpr float min = -6.0f;
                
                phys->physicsData.velocity = {MathUtils::randomNum(min, -min), MathUtils::randomNum(min, -min), MathUtils::randomNum(min, -min)};
        
                ball->SetMesh(MeshManager::Get().GetAssetByName("sphere"));
                ball->SetMaterial(MaterialManager::Get().GetAssetByName("emerald"));
                const glm::vec3 pos = {2.0f * static_cast<float>(i) + static_cast<float>(4 * i), 2.0f * static_cast<float>(j) + static_cast<float>(4 * j), -20.0f};
                ball->SetPosition(pos);
                ball->SetScale({mass, mass, mass});
            }
        }
    
        return scene;
    };

    auto scene3 = []
    {
        auto scene = std::make_unique<Scene>();
    
        scene->AddObject<PhysicsSystem>();
    
        auto camera = scene->AddObject<CameraEntity>("Camera");
        auto cameraComp = camera->GetCameraComponent();
        cameraComp->SetAsActiveCamera();
        cameraComp->lockRotation = true;
        
        auto light = scene->AddObject<DirectionalLightObject>("Directional Light");
        light->SetDirection({0.1f, 0.1f, -40.0f});
        light->lightData.ambient = 1.0f;
    
        for (int i = 0; i < 14; i++)
        {
            for (int j = 0; j < 14; j++)
            {
                auto plane = scene->AddObject<MeshEntity>();
                auto phys = plane->AddComponent<PhysicsComponent>();
                phys->physicsData.enableGravity = false;
                phys->physicsData.applyFriction = false;
                float vel = 7.0f;
                phys->physicsData.velocity = {MathUtils::randomNum(-vel, vel), MathUtils::randomNum(-vel, vel), 0.0f};
    
                plane->SetMesh(MeshManager::Get().GetAssetByName("sphere"));
                plane->SetMaterial(MaterialManager::Get().GetAssetByName("chrome"));
                plane->SetPosition({i*2, j*2, -2.0f});
                plane->SetRotation({90.0f, 0.0f, 0.0f});
                plane->SetScale({0.5f, 0.5f, 0.5f});
            }
        }
    
    
        return scene;
    };

    SceneManager::RegisterScene("DefaultScene", defaultScene);
    SceneManager::RegisterScene("Scene2", scene2);
    SceneManager::RegisterScene("Scene3", scene3);

    SceneManager::SetDefaultScene("Scene3");
}