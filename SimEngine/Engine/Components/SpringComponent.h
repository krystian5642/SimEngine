#pragma once
#include "RenderComponent.h"
#include "Rendering/Core/Line.h"

class SpringComponent : public RenderComponent
{
public:
    SpringComponent(ObjectBase* parent, Scene* scene, const std::string& name);
    
    void Draw() const override;
    
    std::unique_ptr<Line> springLine;

    void SetStart(const glm::vec3& newStart);
    const glm::vec3& GetStart() const { return start; }
    
    void SetSpringLength(float newSpringLength);
    float GetSpringLength() const { return springLength; }
    
    void SetDeltaSpringLength(float newDeltaSpringLength);
    float GetDeltaSpringLength() const { return deltaSpringLength; }
    
private:
    void CreateSpring();
    
    glm::vec3 start{0.0f};
    float springLength{4.0f};
    float deltaSpringLength{0.01f};
};
