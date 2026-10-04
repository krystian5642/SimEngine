#pragma once

template<class AssetClass>
class AssetManager
{
    using CreateAssetFunc = std::function<std::shared_ptr<AssetClass>()>;
public:
    std::shared_ptr<AssetClass> GetAssetByName(const std::string& name, bool createNew = false);
    
    void RegisterCreateAsset(const std::string& name, CreateAssetFunc createFunc);

protected:
    AssetManager() = default;
    
    std::shared_ptr<AssetClass> CreateAssetByName(const std::string& name);
    std::shared_ptr<AssetClass> FindInLoadedAssetsByName(const std::string& name);
    
    std::unordered_map<std::string, std::weak_ptr<AssetClass>> loadedAssets;
    std::unordered_map<std::string, CreateAssetFunc> createAssetFuncs;

};

template <class AssetClass>
std::shared_ptr<AssetClass> AssetManager<AssetClass>::GetAssetByName(const std::string& name, bool createNew)
{
    if (createNew)
    {
        return CreateAssetByName(name);
    }
    
    auto asset = FindInLoadedAssetsByName(name);
    if (asset)
    {
        return asset;
    }
    
    asset = CreateAssetByName(name);
    loadedAssets[name] = asset;
    return asset;
}

template <class AssetClass>
void AssetManager<AssetClass>::RegisterCreateAsset(const std::string& name, CreateAssetFunc createFunc)
{
    createAssetFuncs[name] = createFunc;
}

template <class AssetClass>
std::shared_ptr<AssetClass> AssetManager<AssetClass>::CreateAssetByName(const std::string& name)
{
    auto createFuncIt = createAssetFuncs.find(name);
    if (createFuncIt == createAssetFuncs.end())
    {
        throw std::runtime_error("Can't find asset with name : " + name);
    }
    return createFuncIt->second();
}

template <class AssetClass>
std::shared_ptr<AssetClass> AssetManager<AssetClass>::FindInLoadedAssetsByName(const std::string& name)
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
    return nullptr;
}
