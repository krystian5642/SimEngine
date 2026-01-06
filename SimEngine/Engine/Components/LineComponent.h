#pragma once

#include "RenderComponent.h"
#include "Rendering/Core/Line.h"

namespace SimEngine
{
    class Line;
    
    class LineComponent : public RenderComponent
    {
    public:
        LineComponent(ObjectBase* parent, Scene* scene, const std::string& name);
        ~LineComponent() override;
        
        void Draw(const std::shared_ptr<const Shader>& shader, bool visualPass) const override;
        
        const std::vector<glm::vec3>& GetPoints() const { return line->GetPoints(); }
        void SetPoints(const std::vector<glm::vec3>& newPoints) { line->SetPoints(newPoints); }
        
        const glm::vec3& GetColor() const { return line->GetColor(); }
        void SetColor(const glm::vec3& newColor) { line->SetColor(newColor); }
        
        float GetThickness() const { return line->GetThickness(); }
        void SetThickness(float newThickness) { line->SetThickness(newThickness); }
        
        float GetMaxLenght() const { return line->GetMaxLenght(); }
        void SetMaxLenght(float newMaxLenght) { line->SetMaxLenght(newMaxLenght); }
        
        void ClearPoints() { line->ClearPoints(); }
        
        void AddPoint(const glm::vec3& point) { line->AddPoint(point); }
        
    protected:
        std::unique_ptr<Line> line;
    };
}
