#pragma once
#include "SceneComponent.h"
#include "Rendering/Core/Material.h"
#include "Rendering/UniformNames.h"

class Shader;
class Mesh;

namespace UniformNames
{
    UNIFORM_NAME arrowColor = "arrowColor";
}

class VectorVisualizerComponent : public RenderComponent
{
public:
    VectorVisualizerComponent(const SceneObjectParams& params);
    
    void Tick(float deltaTime) override;
    
    void Draw() const override;
    bool IsTransparent() const override;
    
    glm::vec3 GetUIColor() const override { return color; }
    
    void SetStart(const glm::vec3& newStart);
    void SetDirection(const glm::vec3& newDirection);
    
    glm::vec4 color{1.0f, 0.0f, 0.0f, 1.0f};
    float scaleLenghtFactor{1.0f};
    float scaleFactor{1.0f};
    bool useParentLocationAsStart{false};
    
private:
    std::shared_ptr<Mesh> cylinderMesh;
    std::shared_ptr<Mesh> coneMesh;
    std::shared_ptr<Shader> shader;
    
    Transform cylinderTransform;
    Transform coneTransform;
    
    glm::vec3 currentDirection{1.0f, 0.0f, 0.0f};
};
