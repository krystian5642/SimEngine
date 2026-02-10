#include "MeshManager.h"

#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"

#include "Rendering/Core/Mesh.h"
#include "Rendering/Renderer/Renderer.h"


MeshManager& MeshManager::Get()
{
    static MeshManager meshManager;
    return meshManager;
}

MeshPtr MeshManager::LoadMesh(const std::string& path)
{
    std::vector<float> vertices;
    std::vector<unsigned int> indices;
    
    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
    std::string warning, error;

    bool success = tinyobj::LoadObj(&attrib, &shapes, &materials, &warning, &error, path.c_str());
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
            glm::vec3 position = {
                attrib.vertices[3 * index.vertex_index],
                attrib.vertices[3 * index.vertex_index + 1],
                attrib.vertices[3 * index.vertex_index + 2]
            };

            min = glm::min(min, position);
            max = glm::max(max, position);
            
            glm::vec2 uv = {
                attrib.texcoords[2 * index.texcoord_index],
                attrib.texcoords[2 * index.texcoord_index + 1]
            };

            glm::vec3 normal = {
                attrib.normals[3 * index.normal_index],
                attrib.normals[3 * index.normal_index + 1],
                attrib.normals[3 * index.normal_index + 2]
            };

            vertices.push_back(position.x);
            vertices.push_back(position.y);
            vertices.push_back(position.z);
            vertices.push_back(uv.x);
            vertices.push_back(uv.y);
            vertices.push_back(normal.x);
            vertices.push_back(normal.y);
            vertices.push_back(normal.z);
            
            indices.push_back(static_cast<unsigned int>(indices.size()));
        }
    }

    auto center = (max + min) / 2.0f;
    for (size_t i = 0; i < vertices.size(); i += 8)
    {
        vertices[i] -= center.x;
        vertices[i + 1] -= center.y;
        vertices[i + 2] -= center.z;
    }
    
    return Mesh::CreateMesh({vertices, indices});
}

MeshManager::MeshManager() 
    : AssetManager<Mesh>()
{
    createAssetFuncs["cube"] = &MeshManager::LoadCube;
    createAssetFuncs["sphere"] = &MeshManager::LoadSphere;
    createAssetFuncs["plane"] = &MeshManager::LoadPlane;
    createAssetFuncs["skybox"] = &MeshManager::LoadSkybox;
    createAssetFuncs["planet"] = &MeshManager::LoadPlanet;
}

MeshPtr MeshManager::LoadCube()
{
    return LoadMesh("Models/cube/cube.obj");
}

MeshPtr MeshManager::LoadSphere()
{
    return LoadMesh("Models/XXR_BS_T_01/XXR_B_BLOODSTONE_002.obj");
}

MeshPtr MeshManager::LoadPlane()
{
    const std::vector vertices = 
    {
        -1.0f, 0.0f, 1.0f,     0.0f, 1.0f,  0.0f, 1.0f, 0.0f,
        1.0f, 0.0f, 1.0f,      1.0f, 1.0f,  0.0f, 1.0f, 0.0f,
        -1.0f, 0.0f, -1.0f,    0.0f, 0.0f,  0.0f, 1.0f, 0.0f,
        1.0f, 0.0f, -1.0f,     1.0f, 0.0f,  0.0f, 1.0f, 0.0f,
    };
    
    const std::vector<unsigned int> indices = {
        0,1,2,
        1,3,2
    };
    
    return Mesh::CreateMesh({vertices, indices});
}

MeshPtr MeshManager::LoadSkybox()
{
    std::vector skyboxVertices = {
        -1.0f, 1.0f, -1.0f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
        -1.0f, -1.0f, -1.0f,	0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
        1.0f, 1.0f, -1.0f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
        1.0f, -1.0f, -1.0f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,

        -1.0f, 1.0f, 1.0f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
        1.0f, 1.0f, 1.0f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
        -1.0f, -1.0f, 1.0f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
        1.0f, -1.0f, 1.0f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f
    };
    
    std::vector<unsigned int> skyboxIndices = {
        // front
        0, 1, 2,
        2, 1, 3,
        
        // right
        2, 3, 5,
        5, 3, 7,
        
        // back
        5, 7, 4,
        4, 7, 6,
        
        // left
        4, 6, 0,
        0, 6, 1,
        
        // top
        4, 0, 5,
        5, 0, 2,
        
        // bottom
        1, 6, 3,
        3, 6, 7
    };
    
    return Mesh::CreateMesh({skyboxVertices, skyboxIndices});
}

MeshPtr MeshManager::LoadPlanet()
{
    return LoadMesh("Models/Planet/planet.obj");
}