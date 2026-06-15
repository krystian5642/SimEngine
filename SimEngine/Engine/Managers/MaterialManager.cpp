#include "MaterialManager.h"

#include "Core/App.h"
#include "Rendering/Core/Material.h"

MaterialManager& MaterialManager::Get()
{
    static MaterialManager materialLibrary;
    return materialLibrary;
}

MaterialManager::MaterialManager() 
    : AssetManager<Material>()
{
    createAssetFuncs["emerald"] = [] 
    {
        MaterialData data;
        
        data.color = {0.07568f, 0.61424f, 0.07568f};
        data.shader = App::Get().renderer.GetBaseShader();
        
        return std::make_shared<Material>(data);
    };

    createAssetFuncs["jade"] = [] 
    {
        MaterialData data;
        
        data.color = {0.54f, 0.89f, 0.63f};
        data.shader = App::Get().renderer.GetBaseShader();
        
        return std::make_shared<Material>(data);
    };

    createAssetFuncs["obsidian"] = [] 
    {
        MaterialData data;
        
        data.color = {0.18275f, 0.17f, 0.22525f};
        data.shader = App::Get().renderer.GetBaseShader();
        
        return std::make_shared<Material>(data);
    };

    createAssetFuncs["pearl"] = [] 
    {
        MaterialData data;
        
        data.color = {1.0f, 0.829f, 0.829f};
        data.shader = App::Get().renderer.GetBaseShader();
        
        return std::make_shared<Material>(data);
    };

    createAssetFuncs["ruby"] = [] 
    {
        MaterialData data;
        
        data.color = {0.61424f, 0.04136f, 0.04136f};
        data.shader = App::Get().renderer.GetBaseShader();
        
        return std::make_shared<Material>(data);
    };

    createAssetFuncs["turquoise"] = [] 
    {
        MaterialData data;
        
        data.color = {0.396f, 0.74151f, 0.69102f};
        data.shader = App::Get().renderer.GetBaseShader();
        
        return std::make_shared<Material>(data);
    };

    createAssetFuncs["brass"] = [] 
    {
        MaterialData data;
        
        data.color = {0.780392f, 0.568627f, 0.113725f};
        data.shader = App::Get().renderer.GetBaseShader();
        
        return std::make_shared<Material>(data);
    };

    createAssetFuncs["bronze"] = [] 
    {
        MaterialData data;
        
        data.color = {0.714f, 0.4284f, 0.18144f};
        data.shader = App::Get().renderer.GetBaseShader();
        
        return std::make_shared<Material>(data);
    };

    createAssetFuncs["chrome"] = [] 
    {
        MaterialData data;
        
        data.color = {0.4f, 0.4f, 0.4f};
        data.shader = App::Get().renderer.GetBaseShader();
        
        return std::make_shared<Material>(data);
    };

    createAssetFuncs["copper"] = [] 
    {
        MaterialData data;
        
        data.color = {0.7038f, 0.27048f, 0.0828f};
        data.shader = App::Get().renderer.GetBaseShader();
        
        return std::make_shared<Material>(data);
    };

    createAssetFuncs["gold"] = [] 
    {
        MaterialData data;
        
        data.color = {0.75164f, 0.60648f, 0.22648f};
        data.shader = App::Get().renderer.GetBaseShader();
        
        return std::make_shared<Material>(data);
    };

    createAssetFuncs["silver"] = [] 
    {
        MaterialData data;
        
        data.color = {0.50754f, 0.50754f, 0.50754f};
        data.shader = App::Get().renderer.GetBaseShader();
        
        return std::make_shared<Material>(data);
    };

    createAssetFuncs["concrete"] = []
    {
        MaterialData data;
        
        data.color = {0.1038f, 0.17048f, 0.1828f};
        data.shader = App::Get().renderer.GetBaseShader();
        
        return std::make_shared<Material>(data);
    };
}