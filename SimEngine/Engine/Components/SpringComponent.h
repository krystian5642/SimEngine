#pragma once
#include "RenderComponent.h"
#include "Rendering/Core/Line.h"

class SpringComponent : public RenderComponent
{
public:
    SpringComponent(ObjectBase* parent, Scene* scene, const std::string& name);
    
    void Draw() const override;

    void SetCoilsNum(int newCoilsNum);
    int GetCoilsNum() const { return coilsNum; }
    
    void SetStart(const glm::vec3& newStart);
    const glm::vec3& GetStart() const { return start; }
    
    void SetEnd(const glm::vec3& newEnd);
    const glm::vec3& GetEnd() const { return end; }
    
    void SetRadius(float newRadius);
    float GetRadius() const { return radius; }
    
    void SetDeltaAngle(float newDeltaAngle);
    float GetDeltaAngle() const { return deltaAngle; }
    
    Line* GetSpringLine() const { return springLine.get(); }
    
private:
    void RecreateSpringPoints() const;
    
    std::unique_ptr<Line> springLine;
    
    // Spring properties
    int coilsNum{10};
    glm::vec3 start{0.0f};
    glm::vec3 end{0.0f, -5.0f, 0.0f};
    float radius{1.0f};
    float deltaAngle{0.1f};
    
    mutable bool bShouldRecreateSpringPoints{false};
};
