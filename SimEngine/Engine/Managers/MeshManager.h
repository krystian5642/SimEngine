#pragma once

#include "AssetManager.h"

class Mesh;

class MeshManager : public AssetManager<Mesh>
{
public:
    static MeshManager& Get();

    static MeshPtr LoadMesh(const std::string& path);
    
private:
    MeshManager();

    static MeshPtr LoadCube();
    static MeshPtr LoadSphere();
    static MeshPtr LoadPlane();
    static MeshPtr LoadSkybox();
    static MeshPtr LoadPlanet();
};
