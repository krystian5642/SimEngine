#include "MaterialManager.h"

#include "Rendering/Core/Material.h"

namespace SimEngine
{
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
            MaterialResources resources;
            MaterialData& data = resources.data;
            
            data.ambient = {0.0215f, 0.1745f, 0.0215f};
            data.diffuse = {0.07568f, 0.61424f, 0.07568f};
            data.specular = {0.633f, 0.727811f, 0.633f};
            data.shininess = 0.6f * 128.0f;
            
            return std::make_shared<Material>(resources);
        };

        createAssetFuncs["jade"] = [] 
        {
            MaterialResources resources;
            MaterialData& data = resources.data;
            
            data.ambient = {0.135f, 0.2225f, 0.1575f};
            data.diffuse = {0.54f, 0.89f, 0.63f};
            data.specular = {0.316228f, 0.316228f, 0.316228f};
            data.shininess = 0.1f * 128.0f;
            
            return std::make_shared<Material>(resources);
        };

        createAssetFuncs["obsidian"] = [] 
        {
            MaterialResources resources;
            MaterialData& data = resources.data;
            
            data.ambient = {0.05375f, 0.05f, 0.06625f};
            data.diffuse = {0.18275f, 0.17f, 0.22525f};
            data.specular = {0.332741f, 0.328634f, 0.346435f};
            data.shininess = 0.3f * 128.0f;
            
            return std::make_shared<Material>(resources);
        };

        createAssetFuncs["pearl"] = [] 
        {
            MaterialResources resources;
            MaterialData& data = resources.data;
            
            data.ambient = {0.25f, 0.20725f, 0.20725f};
            data.diffuse = {1.0f, 0.829f, 0.829f};
            data.specular = {0.296648f, 0.296648f, 0.296648f};
            data.shininess = 0.088f * 128.0f;
            
            return std::make_shared<Material>(resources);
        };

        createAssetFuncs["ruby"] = [] 
        {
            MaterialResources resources;
            MaterialData& data = resources.data;
            
            data.ambient = {0.1745f, 0.01175f, 0.01175f};
            data.diffuse = {0.61424f, 0.04136f, 0.04136f};
            data.specular = {0.727811f, 0.626959f, 0.626959f};
            data.shininess = 0.6f * 128.0f;
            
            return std::make_shared<Material>(resources);
        };

        createAssetFuncs["turquoise"] = [] 
        {
            MaterialResources resources;
            MaterialData& data = resources.data;
            
            data.ambient = {0.1f, 0.18725f, 0.1745f};
            data.diffuse = {0.396f, 0.74151f, 0.69102f};
            data.specular = {0.297254f, 0.30829f, 0.306678f};
            data.shininess = 0.1f * 128.0f;
            
            return std::make_shared<Material>(resources);
        };

        createAssetFuncs["brass"] = [] 
        {
            MaterialResources resources;
            MaterialData& data = resources.data;
            
            data.ambient = {0.329412f, 0.223529f, 0.027451f};
            data.diffuse = {0.780392f, 0.568627f, 0.113725f};
            data.specular = {0.992157f, 0.941176f, 0.807843f};
            data.shininess = 0.21794872f * 128.0f;
            
            return std::make_shared<Material>(resources);
        };

        createAssetFuncs["bronze"] = [] 
        {
            MaterialResources resources;
            MaterialData& data = resources.data;
            
            data.ambient = {0.2125f, 0.1275f, 0.054f};
            data.diffuse = {0.714f, 0.4284f, 0.18144f};
            data.specular = {0.393548f, 0.271906f, 0.166721f};
            data.shininess = 0.2f * 128.0f;
            
            return std::make_shared<Material>(resources);
        };

        createAssetFuncs["chrome"] = [] 
        {
            MaterialResources resources;
            MaterialData& data = resources.data;
            
            data.ambient = {0.25f, 0.25f, 0.25f};
            data.diffuse = {0.4f, 0.4f, 0.4f};
            data.specular = {0.774597f, 0.774597f, 0.774597f};
            data.shininess = 0.6f * 128.0f;
            
            return std::make_shared<Material>(resources);
        };

        createAssetFuncs["copper"] = [] 
        {
            MaterialResources resources;
            MaterialData& data = resources.data;
            
            data.ambient = {0.19125f, 0.0735f, 0.0225f};
            data.diffuse = {0.7038f, 0.27048f, 0.0828f};
            data.specular = {0.256777f, 0.137622f, 0.086014f};
            data.shininess = 0.1f * 128.0f;
            
            return std::make_shared<Material>(resources);
        };

        createAssetFuncs["gold"] = [] 
        {
            MaterialResources resources;
            MaterialData& data = resources.data;
            
            data.ambient = {0.24725f, 0.1995f, 0.0745f};
            data.diffuse = {0.75164f, 0.60648f, 0.22648f};
            data.specular = {0.628281f, 0.555802f, 0.366065f};
            data.shininess = 0.4f * 128.0f;
            
            return std::make_shared<Material>(resources);
        };

        createAssetFuncs["silver"] = [] 
        {
            MaterialResources resources;
            MaterialData& data = resources.data;
            
            data.ambient = {0.19225f, 0.19225f, 0.19225f};
            data.diffuse = {0.50754f, 0.50754f, 0.50754f};
            data.specular = {0.508273f, 0.508273f, 0.508273f};
            data.shininess = 0.4f * 128.0f;
            
            return std::make_shared<Material>(resources);
        };

        createAssetFuncs["black plastic"] = [] 
        {
            MaterialResources resources;
            MaterialData& data = resources.data;
            
            data.ambient = {0.0f, 0.0f, 0.0f};
            data.diffuse = {0.01f, 0.01f, 0.01f};
            data.specular = {0.5f, 0.5f, 0.5f};
            data.shininess = 0.25f * 128.0f;
            
            return std::make_shared<Material>(resources);
        };

        createAssetFuncs["cyan plastic"] = [] 
        {
            MaterialResources resources;
            MaterialData& data = resources.data;
            
            data.ambient = {0.0f, 0.1f, 0.06f};
            data.diffuse = {0.0f, 0.50980392f, 0.50980392f};
            data.specular = {0.50196078f, 0.50196078f, 0.50196078f};
            data.shininess = 0.25f * 128.0f;
            
            return std::make_shared<Material>(resources);
        };

        createAssetFuncs["green plastic"] = [] 
        {
            MaterialResources resources;
            MaterialData& data = resources.data;
            
            data.ambient = {0.0f, 0.0f, 0.0f};
            data.diffuse = {0.1f, 0.35f, 0.1f};
            data.specular = {0.45f, 0.55f, 0.45f};
            data.shininess = 0.25f * 128.0f;
            
            return std::make_shared<Material>(resources);
        };

        createAssetFuncs["red plastic"] = [] 
        {
            MaterialResources resources;
            MaterialData& data = resources.data;
            
            data.ambient = {0.0f, 0.0f, 0.0f};
            data.diffuse = {0.5f, 0.0f, 0.0f};
            data.specular = {0.7f, 0.6f, 0.6f};
            data.shininess = 0.25f * 128.0f;
            
            return std::make_shared<Material>(resources);
        };

        createAssetFuncs["white plastic"] = [] 
        {
            MaterialResources resources;
            MaterialData& data = resources.data;
            
            data.ambient = {0.0f, 0.0f, 0.0f};
            data.diffuse = {0.55f, 0.55f, 0.55f};
            data.specular = {0.7f, 0.7f, 0.7f};
            data.shininess = 0.25f * 128.0f;
            
            return std::make_shared<Material>(resources);
        };

        createAssetFuncs["yellow plastic"] = [] 
        {
            MaterialResources resources;
            MaterialData& data = resources.data;
            
            data.ambient = {0.0f, 0.0f, 0.0f};
            data.diffuse = {0.5f, 0.5f, 0.0f};
            data.specular = {0.6f, 0.6f, 0.5f};
            data.shininess = 0.25f * 128.0f;
            
            return std::make_shared<Material>(resources);
        };

        createAssetFuncs["black rubber"] = [] 
        {
            MaterialResources resources;
            MaterialData& data = resources.data;
            
            data.ambient = {0.02f, 0.02f, 0.02f};
            data.diffuse = {0.01f, 0.01f, 0.01f};
            data.specular = {0.4f, 0.4f, 0.4f};
            data.shininess = 0.078125f * 128.0f;
            
            return std::make_shared<Material>(resources);
        };

        createAssetFuncs["cyan rubber"] = [] 
        {
            MaterialResources resources;
            MaterialData& data = resources.data;
            
            data.ambient = {0.0f, 0.05f, 0.05f};
            data.diffuse = {0.4f, 0.5f, 0.5f};
            data.specular = {0.04f, 0.7f, 0.7f};
            data.shininess = 0.078125f * 128.0f;
            
            return std::make_shared<Material>(resources);
        };

        createAssetFuncs["green rubber"] = [] 
        {
            MaterialResources resources;
            MaterialData& data = resources.data;
            
            data.ambient = {0.0f, 0.05f, 0.0f};
            data.diffuse = {0.4f, 0.5f, 0.4f};
            data.specular = {0.04f, 0.7f, 0.04f};
            data.shininess = 0.078125f * 128.0f;
            
            return std::make_shared<Material>(resources);
        };

        createAssetFuncs["red rubber"] = [] 
        {
            MaterialResources resources;
            MaterialData& data = resources.data;
            
            data.ambient = {0.05f, 0.0f, 0.0f};
            data.diffuse = {0.5f, 0.4f, 0.4f};
            data.specular = {0.7f, 0.04f, 0.04f};
            data.shininess = 0.078125f * 128.0f;
            
            return std::make_shared<Material>(resources);
        };

        createAssetFuncs["white rubber"] = [] 
        {
            MaterialResources resources;
            MaterialData& data = resources.data;
            
            data.ambient = {0.05f, 0.05f, 0.05f};
            data.diffuse = {0.5f, 0.5f, 0.5f};
            data.specular = {0.7f, 0.7f, 0.7f};
            data.shininess = 0.078125f * 128.0f;
            
            return std::make_shared<Material>(resources);
        };

        createAssetFuncs["yellow rubber"] = [] 
        {
            MaterialResources resources;
            MaterialData& data = resources.data;
            
            data.ambient = {0.05f, 0.05f, 0.0f};
            data.diffuse = {0.5f, 0.5f, 0.4f};
            data.specular = {0.7f, 0.7f, 0.04f};
            data.shininess = 0.078125f * 128.0f;
            
            return std::make_shared<Material>(resources);
        };

        createAssetFuncs["concrete"] = []
        {
            MaterialResources resources;
            MaterialData& data = resources.data;
            
            data.ambient  = { 0.2f, 0.2f, 0.2f };
            data.diffuse  = { 0.5f, 0.5f, 0.5f };
            data.specular = { 0.1f, 0.1f, 0.1f };  
            data.shininess = 16.0f;
            
            return std::make_shared<Material>(resources);
        };

    }
}
