#pragma once

#include "AssetManager.h"

class Mesh;

class MeshManager : public AssetManager<Mesh>
{
public:
    static MeshManager& Get();

    static std::shared_ptr<Mesh> LoadMesh(const std::string& path);
    
private:
    MeshManager();

    static std::shared_ptr<Mesh> LoadCube();
    static std::shared_ptr<Mesh> LoadSphere();
    static std::shared_ptr<Mesh> LoadPlane();
};
