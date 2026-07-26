#pragma once

#include "AssetManager.h"

class Mesh;

class MeshManager : public AssetManager<Mesh>
{
public:
    static MeshManager& Get();

    static std::shared_ptr<Mesh> LoadMesh(const std::string& path, bool autoCenterPivot = true);
    
private:
    MeshManager();

    static std::shared_ptr<Mesh> LoadCube();
    static std::shared_ptr<Mesh> LoadSphere();
    static std::shared_ptr<Mesh> LoadPlane();
    static std::shared_ptr<Mesh> LoadCylinder();
    static std::shared_ptr<Mesh> LoadArrowCylinder();
    static std::shared_ptr<Mesh> LoadArrowCone();
};
