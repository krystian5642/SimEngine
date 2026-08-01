#include "SpringComponent.h"

SpringComponent::SpringComponent(ObjectBase* parent, Scene* scene, const std::string& name)
    : RenderComponent(parent, scene, name)
{
    springLine = std::make_unique<Line>();
    RecreateSpringPoints();
}

void SpringComponent::Draw() const
{
    springLine->Draw();
}

void SpringComponent::SetCoilsNum(int newCoilsNum)
{
    coilsNum = newCoilsNum;
    RecreateSpringPoints();
}

void SpringComponent::SetStart(const glm::vec3& newStart)
{
    start = newStart;
    RecreateSpringPoints();
}

void SpringComponent::SetEnd(const glm::vec3& newEnd)
{
    end = newEnd;
    RecreateSpringPoints();
}

void SpringComponent::SetRadius(float newRadius)
{
    radius = newRadius;
    RecreateSpringPoints();
}

void SpringComponent::SetDeltaAngle(float newDeltaAngle)
{
    deltaAngle = newDeltaAngle;
    RecreateSpringPoints();
}

void SpringComponent::RecreateSpringPoints()
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
}
