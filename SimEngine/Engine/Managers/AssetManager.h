#pragma once

namespace SimEngine
{
    template<class AssetClass>
    class AssetManager
    {
        using CreateAssetFunc = std::function<std::shared_ptr<AssetClass>()>;
    public:
        std::shared_ptr<AssetClass> GetAssetByName(const std::string& name);
        
        void RegisterCreateAsset(const std::string& name, CreateAssetFunc createFunc);
    
    protected:
        AssetManager() = default;
        
        std::unordered_map<std::string, std::weak_ptr<AssetClass>> loadedAssets;
        std::unordered_map<std::string, CreateAssetFunc> createAssetFuncs;
    
    };

    template <class AssetClass>
    std::shared_ptr<AssetClass> AssetManager<AssetClass>::GetAssetByName(const std::string& name)
    {
        auto it = loadedAssets.find(name);
        if (it != loadedAssets.end())
        {
            auto asset = it->second.lock();
            if (asset)
            {
                return asset;
            }
            loadedAssets.erase(it);
        }
    
        auto createFuncIt = createAssetFuncs.find(name);
        if (createFuncIt == createAssetFuncs.end())
        {
            std::cout << "WARNING: Trying to get an asset but no create func is specified !!" << std::endl;
            return nullptr;
        }
    
        auto asset = createFuncIt->second();
        loadedAssets[name] = asset;
        return asset;
    }

    template <class AssetClass>
    void AssetManager<AssetClass>::RegisterCreateAsset(const std::string& name, CreateAssetFunc createFunc)
    {
        createAssetFuncs[name] = createFunc;
    }
}
