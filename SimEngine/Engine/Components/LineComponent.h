#pragma once

#include "RenderComponent.h"
#include "Rendering/Core/Line.h"

class Line;

class LineComponent : public RenderComponent
{
public:
    LineComponent(ObjectBase* parent, Scene* scene, const std::string& name);
    
    void Tick(float deltaTime) override;
    void Draw() const override;
    
    std::unique_ptr<Line> line;
    
    bool followParent{false};
};
