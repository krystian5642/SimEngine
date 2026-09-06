#pragma once

#include "MeshComponent.h"

class SegmentComponent : public MeshComponent
{
public:
    SegmentComponent(ObjectBase* parent, Scene* scene, const std::string& name);
   
    void Tick(float deltaTime) override;
    
    void SetStart(const glm::vec3& newStart);
    void SetEnd(const glm::vec3& newEnd);
    
    bool useParentLocationAsStart{false};
};
