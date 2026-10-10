#pragma once
#include "RenderComponent.h"
#include "Rendering/Core/Line.h"

class SpringComponent : public RenderComponent
{
public:
    SpringComponent(const SceneObjectParams& params);
    
    void Draw() const override;
    bool IsTransparent() const override;
    void DrawUI() override;
    
    glm::vec3 GetUIColor() const override;
    
    Line* GetSpringLine() const { return springLine.get(); }
    
    PROPERTY(IntProperty, coilsNum, 10, 1, 40)
    PROPERTY(Vec3Property, start, glm::vec3{0.0f})
    PROPERTY(Vec3Property, end, glm::vec3{0.0f, -5.0f, 0.0f})
    PROPERTY(FloatProperty, radius, 1.0f, 0.01f, 10.0f, "%.3f m")
    PROPERTY(FloatProperty, deltaAngle, 0.1f, 0.01f, 90.0f, "%.3f deg")
    
private:
    template<class T>
    void OnPropertyChanged(const T&)
    {
        RecreateSpringPoints();
    }
    
    void RecreateSpringPoints() const;
    
    std::unique_ptr<Line> springLine;
};
