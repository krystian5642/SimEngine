#pragma once

class InstancedMesh;
class ShadowMap;
class Mesh;
class Material;
class Skybox;
class Line;
class Texture;
class Shader;

class Renderer;

using TexturePtr = std::shared_ptr<Texture>;
using LinePtr = std::shared_ptr<Line>;
using MeshPtr = std::shared_ptr<Mesh>;
using MaterialPtr = std::shared_ptr<Material>;
using SkyboxPtr = std::shared_ptr<Skybox>;
using ShadowMapPtr = std::shared_ptr<ShadowMap>;
using ShaderPtr = std::shared_ptr<Shader>;
using InstancedMeshPtr = std::shared_ptr<InstancedMesh>;

using RendererPtr = std::unique_ptr<Renderer>;