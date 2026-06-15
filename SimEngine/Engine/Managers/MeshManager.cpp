#include "MeshManager.h"

#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"

#include "Rendering/Core/Mesh.h"

MeshManager& MeshManager::Get()
{
    static MeshManager meshManager;
    return meshManager;
}

std::shared_ptr<Mesh> MeshManager::LoadMesh(const std::string& path)
{
    std::vector<VertexData> vertices;
    std::vector<unsigned int> indices;
    
    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
    std::string warning, error;

    const auto success = tinyobj::LoadObj(&attrib, &shapes, &materials, &warning, &error, path.c_str());
    std::cout << error << "\n" << warning;
    if (!success)
    {
        return nullptr;
    }

    glm::vec3 min{FLT_MAX};
    glm::vec3 max{-FLT_MAX};
    
    for (const auto& shape : shapes)
    {
        for (const auto& index : shape.mesh.indices)
        {
            VertexData vertex;
            
            vertex.position = {
                attrib.vertices[3 * index.vertex_index],
                attrib.vertices[3 * index.vertex_index + 1],
                attrib.vertices[3 * index.vertex_index + 2]
            };

            min = glm::min(min, vertex.position);
            max = glm::max(max, vertex.position);
            
            vertex.uv = {
                attrib.texcoords[2 * index.texcoord_index],
                attrib.texcoords[2 * index.texcoord_index + 1]
            };

            vertex.normal = {
                attrib.normals[3 * index.normal_index],
                attrib.normals[3 * index.normal_index + 1],
                attrib.normals[3 * index.normal_index + 2]
            };

            vertices.push_back(vertex);
            
            indices.push_back(static_cast<unsigned int>(indices.size()));
        }
    }
    
    auto center = (max + min) / 2.0f;
    for (auto& vertex : vertices)
    {
        vertex.position -= center;
    }
    
    return std::make_shared<Mesh>(MeshData{vertices, indices});
}

MeshManager::MeshManager() 
    : AssetManager<Mesh>()
{
    createAssetFuncs["cube"] = &MeshManager::LoadCube;
    createAssetFuncs["sphere"] = &MeshManager::LoadSphere;
    createAssetFuncs["plane"] = &MeshManager::LoadPlane;
}

std::shared_ptr<Mesh> MeshManager::LoadCube()
{
    return LoadMesh("Models/cube/cube.obj");
}

std::shared_ptr<Mesh> MeshManager::LoadSphere()
{
    return LoadMesh("Models/XXR_BS_T_01/XXR_B_BLOODSTONE_002.obj");
}

std::shared_ptr<Mesh> MeshManager::LoadPlane()
{
    const std::vector<VertexData> vertices =
    {
        { {-1.0f, 0.0f,  1.0f}, {0.0f, 1.0f}, {0.0f, 1.0f, 0.0f} },
        { { 1.0f, 0.0f,  1.0f}, {1.0f, 1.0f}, {0.0f, 1.0f, 0.0f} },
        { {-1.0f, 0.0f, -1.0f}, {0.0f, 0.0f}, {0.0f, 1.0f, 0.0f}},
        { { 1.0f, 0.0f, -1.0f}, {1.0f, 0.0f}, {0.0f, 1.0f, 0.0f} },
    };
    
    const std::vector<unsigned int> indices = {
        0,1,2,
        1,3,2
    };
    
    return std::make_shared<Mesh>(MeshData{vertices, indices});
}