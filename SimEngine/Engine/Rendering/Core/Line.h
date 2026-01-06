#pragma once
#include "Rendering/UniformNames.h"

namespace SimEngine
{
    class Shader;
    
    namespace UniformNames
    {
        UNIFORM_NAME lineColor = "lineColor";
    }
    
    class Line
    {
    public:
        Line();
        virtual ~Line();
        
        const std::vector<glm::vec3>& GetPoints() const { return points; }
        void SetPoints(const std::vector<glm::vec3>& newPoints);
        
        const glm::vec3& GetColor() const { return color; }
        void SetColor(const glm::vec3& newColor) { color = newColor; }
        
        float GetThickness() const { return thickness; }
        void SetThickness(float newThickness) { thickness = newThickness; }
        
        float GetMaxLenght() const { return maxLength; }
        void SetMaxLenght(float newMaxLength) { maxLength = newMaxLength; }
        
        void ClearPoints() { points.clear(); }
        
        void AddPoint(const glm::vec3& point);
        
        virtual void Draw() const = 0;
        
    protected:
        std::vector<glm::vec3> points;
        std::shared_ptr<Shader> shader;
        
        glm::vec3 color{1.0f};
        float thickness{1.0f};
        float maxLength{-1.0f};
        float currentLength{0.0f};
    };
}
