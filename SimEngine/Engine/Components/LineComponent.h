#pragma once

#include "RenderComponent.h"
#include "Rendering/Core/Line.h"

class Line;

class LineComponent : public RenderComponent
{
public:
    LineComponent(ObjectBase* parent, Scene* scene, const std::string& name);
    
    void Tick(float deltaTime) override;
    void Draw(const std::shared_ptr<const Shader>& shader, bool visualPass) const override;
    
    const std::vector<glm::vec3>& GetPoints() const { return line->GetPoints(); }
    void SetPoints(const std::vector<glm::vec3>& newPoints) { line->SetPoints(newPoints); }
    
    const glm::vec3& GetColor() const { return line->color; }
    void SetColor(const glm::vec3& newColor) { line->color = newColor; }
    
    float GetThickness() const { return line->thickness; }
    void SetThickness(float newThickness) { line->thickness = newThickness; }
    
    float GetMaxLenght() const { return line->maxLength; }
    void SetMaxLenght(float newMaxLenght) { line->maxLength = newMaxLenght; }
    
    void ClearPoints() { line->ClearPoints(); }
    
    void AddPoint(const glm::vec3& point) { line->AddPoint(point); }
    
    bool followParent{false};
protected:
    LinePtr line;
};
