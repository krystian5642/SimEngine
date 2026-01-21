#include "ExampleApp.h"

#include "Scenes.h"
#include "Managers/MaterialManager.h"
#include "Managers/TextureManager.h"
#include "Rendering/Renderer/Renderer.h"
#include "Rendering/Core/Texture.h"
#include "Scene/SceneManager.h"

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

    /*auto defaultScene = []
    {
        const std::vector<std::string> skyboxFaces = {
            "Textures/Skybox/right.jpg",
            "Textures/Skybox/left.jpg",
            "Textures/Skybox/top.jpg",
            "Textures/Skybox/bottom.jpg",
            "Textures/Skybox/front.jpg",
            "Textures/Skybox/back.jpg"
        };
    
        auto scene = std::make_unique<Scene>();
        scene->SetSkybox(Renderer::CreateSkyboxStatic(skyboxFaces));
    
        scene->AddObject<BallLauncher>();
    
        auto camera = scene->AddObject<CameraEntity>("Camera");
        camera->SetAsActiveCamera();
        
        scene->AddObject<PhysicsCollisionSystem>();
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
    
    auto scene6 = [] -> std::unique_ptr<Scene>
    {
        auto scene = std::make_unique<Scene>();
        
        auto camera = scene->AddObject<CameraEntity>("Camera");
        auto cameraComp = camera->GetCameraComponent();
        cameraComp->SetAsActiveCamera();
        cameraComp->SetPosition({0.0f, 0.0f, 30.0f});
        
        auto light = scene->AddObject<DirectionalLightObject>("Directional Light");
        light->SetDirection({0.1f, 0.1f, -40.0f});
        light->lightData.ambient = 1.0f;
    
        /*for (int i = -7; i <= 7; i++)
        {
            for (int j = -7; j <= 7; j++)
            {
                for (int k = -7; k <= 7; k++)
                {
                    //double num = MathUtils::randomNum(0.0, 1.0);
                    if (i*i + j*j + k*k > 49) continue; 
                    
                    auto cube = scene->AddObject<MeshEntity>();
                    cube->SetMesh(MeshManager::Get().GetAssetByName("cube"));
                    cube->SetMaterial(MaterialManager::Get().GetAssetByName("chrome"));
                    cube->SetPosition({i, j, k});
                }
            }
        }#1#
        
        for (int i = -7; i <= 7; i++)
        {
            for (int j = -7; j <= 7; j++)
            {
                //double num = MathUtils::randomNum(0.0, 1.0);
                if (i*i + j*j > 49) continue; 
                    
                auto cube = scene->AddObject<MeshEntity>();
                cube->SetMesh(MeshManager::Get().GetAssetByName("cube"));
                cube->SetMaterial(MaterialManager::Get().GetAssetByName("chrome"));
                cube->SetPosition({i, j, 0});
            }
        }
        
        return scene;
    };*/

    SceneManager::SetDefaultScene(SceneNames::GravityShip);
    SceneManager::RegisterScene(SceneNames::GravityShip, [] -> std::unique_ptr<Scene> { return std::make_unique<GravityShipScene>(); });
    SceneManager::RegisterScene(SceneNames::BallCollision2D, [] -> std::unique_ptr<Scene> { return std::make_unique<BallCollision2DScene>(); });
    SceneManager::RegisterScene(SceneNames::BallCollision3D, [] -> std::unique_ptr<Scene> { return std::make_unique<BallCollision3DScene>(); });
    SceneManager::RegisterScene(SceneNames::SphereGrid, [] -> std::unique_ptr<Scene> { return std::make_unique<SphereGridScene>(); });
}