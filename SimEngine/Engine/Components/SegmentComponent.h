#pragma once

#include "MeshComponent.h"

class SegmentComponent : public MeshComponent
{
public:
    SegmentComponent(ObjectBase* parent, Scene* scene, const std::string& name);
   
    void Tick(float deltaTime) override;
    
    void SetStart(const glm::vec3& newStart);
    void SetEnd(const glm::vec3& newEnd);
    
    void SetThickness(float newThickness);
    float GetThickness() const { return thickness; }
    
    bool useParentLocationAsStart{false};
    
private:
    float thickness{1.0f};
};
