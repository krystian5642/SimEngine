#pragma once

#include "RenderComponent.h"
#include "Rendering/Core/Line.h"

class Line;

class LineComponent : public RenderComponent
{
public:
    LineComponent(const SceneObjectParams& params);
    
    void Tick(float deltaTime) override;
    void Draw() const override;
    
    bool followParent{false};
    
    Line* GetLine() const { return line.get(); }
    
private:
    std::unique_ptr<Line> line;
};
