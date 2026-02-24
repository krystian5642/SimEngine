#include "MeshManager.h"

#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"

#include "Rendering/Core/Mesh.h"

MeshManager& MeshManager::Get()
{
    static MeshManager meshManager;
    return meshManager;
}

MeshPtr MeshManager::LoadMesh(const std::string& path)
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
    
    // calc tangents
    for (int i = 0; i < indices.size(); i += 3)
    {
        const auto idx0 = indices[i];
        const auto idx1 = indices[i + 1];
        const auto idx2 = indices[i + 2];
        
        const auto edge1 = vertices[idx1].position - vertices[idx0].position;
        const auto edge2 = vertices[idx2].position - vertices[idx0].position;
        
        const auto deltaU1 = vertices[idx1].uv.x - vertices[idx0].uv.x;
        const auto deltaV1 = vertices[idx1].uv.y - vertices[idx0].uv.y;
        
        const auto deltaU2 = vertices[idx2].uv.x - vertices[idx0].uv.x;
        const auto deltaV2 = vertices[idx2].uv.y - vertices[idx0].uv.y;
        
        const auto div = 1.0f / (deltaU1 * deltaV2 - deltaU2 * deltaV1);
        
        const auto tangent = (deltaV2 * edge1 - deltaV1 * edge2) * div;
        
        vertices[idx0].tangent += tangent;
        vertices[idx1].tangent += tangent;
        vertices[idx2].tangent += tangent;
    }
    
    auto center = (max + min) / 2.0f;
    for (auto& vertex : vertices)
    {
        vertex.tangent = glm::normalize(vertex.tangent);
        vertex.position -= center;
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
    const std::vector<VertexData> vertices =
    {
        { {-1.0f, 0.0f,  1.0f}, {0.0f, 1.0f}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f, 0.0f} },
        { { 1.0f, 0.0f,  1.0f}, {1.0f, 1.0f}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f, 0.0f} },
        { {-1.0f, 0.0f, -1.0f}, {0.0f, 0.0f}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f, 0.0f} },
        { { 1.0f, 0.0f, -1.0f}, {1.0f, 0.0f}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f, 0.0f} },
    };
    
    const std::vector<unsigned int> indices = {
        0,1,2,
        1,3,2
    };
    
    return Mesh::CreateMesh({vertices, indices});
}

MeshPtr MeshManager::LoadSkybox()
{
    const std::vector<VertexData> skyboxVertices =
    {
        { {-1.0f,  1.0f, -1.0f}, {0,0}, {0,0,0}, {0,0,0} },
        { {-1.0f, -1.0f, -1.0f}, {0,0}, {0,0,0}, {0,0,0} },
        { { 1.0f,  1.0f, -1.0f}, {0,0}, {0,0,0}, {0,0,0} },
        { { 1.0f, -1.0f, -1.0f}, {0,0}, {0,0,0}, {0,0,0} },

        { {-1.0f,  1.0f,  1.0f}, {0,0}, {0,0,0}, {0,0,0} },
        { { 1.0f,  1.0f,  1.0f}, {0,0}, {0,0,0}, {0,0,0} },
        { {-1.0f, -1.0f,  1.0f}, {0,0}, {0,0,0}, {0,0,0} },
        { { 1.0f, -1.0f,  1.0f}, {0,0}, {0,0,0}, {0,0,0} }
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