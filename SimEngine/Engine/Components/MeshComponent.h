#pragma once

#include "SceneComponent.h"
#include "Rendering/Renderer/Renderer.h"

class Mesh;
class Material;

#if ENABLE_TESSELLATION
namespace UniformNames
{
    UNIFORM_NAME tesselationLevel = "tesselationLevel";
}
#endif

class MeshComponent : public SceneComponent
{
public:
    MeshComponent(ObjectBase* parent, Scene* scene, const std::string& name);

    void Draw(const std::shared_ptr<const Shader>& shader, bool visualPass) const override;
    
    MeshPtr mesh;
    MaterialPtr material;
    
    RenderPolygonMode renderPolygonMode{RenderPolygonMode::Fill};
    float lineWidth{1.0f};
    float pointSize{3.0f};
    
#if ENABLE_TESSELLATION
    float tesselationLevel{1.0f};
    bool tessellate{false};
#endif
};
