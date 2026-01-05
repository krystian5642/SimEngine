#include "ExampleApp.h"

#include "BallLauncher.h"
#include "GravitySystem.h"
#include "PlanetSystem.h"
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

ExampleApp::ExampleApp()
{
    using namespace SimEngine;
    
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
        
        return scene;
    };
    
    SceneManager::RegisterScene("DefaultScene", defaultScene);
}

ExampleApp::~ExampleApp()
{
}
