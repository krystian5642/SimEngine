#pragma once

#include "AssetManager.h"

class Mesh;

class MeshManager : public AssetManager<Mesh>
{
public:
    static MeshManager& Get();

private:
    MeshManager();

    static std::shared_ptr<Mesh> LoadMesh(const std::string& path);

    static std::shared_ptr<Mesh> LoadCube();
    static std::shared_ptr<Mesh> LoadSphere();
    static std::shared_ptr<Mesh> LoadPlane();
    static std::shared_ptr<Mesh> LoadSkybox();
    static std::shared_ptr<Mesh> LoadPlanet();
};
