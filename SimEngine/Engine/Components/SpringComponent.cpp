#include "SpringComponent.h"

SpringComponent::SpringComponent(ObjectBase* parent, Scene* scene, const std::string& name)
    : RenderComponent(parent, scene, name)
{
    springLine = std::make_unique<Line>();
    bShouldRecreateSpringPoints = true;
}

void SpringComponent::Draw() const
{
    if (bShouldRecreateSpringPoints)
    {
        RecreateSpringPoints();
    }
    springLine->Draw();
}

void SpringComponent::SetCoilsNum(int newCoilsNum)
{
    coilsNum = newCoilsNum;
    bShouldRecreateSpringPoints = true;
}

void SpringComponent::SetStart(const glm::vec3& newStart)
{
    start = newStart;
    bShouldRecreateSpringPoints = true;
}

void SpringComponent::SetEnd(const glm::vec3& newEnd)
{
    end = newEnd;
    bShouldRecreateSpringPoints = true;
}

void SpringComponent::SetRadius(float newRadius)
{
    radius = newRadius;
    bShouldRecreateSpringPoints = true;
}

void SpringComponent::SetDeltaAngle(float newDeltaAngle)
{
    deltaAngle = newDeltaAngle;
    bShouldRecreateSpringPoints = true;
}

void SpringComponent::RecreateSpringPoints() const
{
    springLine->ClearPoints();
    
    const float springLength = end.y - start.y;
    const float coilHeight = springLength / static_cast<float>(coilsNum);
    const float deltaSpringLength = coilHeight * deltaAngle / glm::two_pi<float>();
    
    glm::vec3 point{radius, 0.0f, 0.0f};
    
    springLine->AddPoint(point + start);
    
    float angle = 0.0f;
    while (glm::abs(point.y + start.y - end.y) > 0.1f)
    {
        point.x = radius * glm::cos(angle);
        point.y += deltaSpringLength;
        point.z = radius * glm::sin(angle);
        
        angle += deltaAngle;
        
        springLine->AddPoint(point + start);
    }
    
    bShouldRecreateSpringPoints = false;
}
