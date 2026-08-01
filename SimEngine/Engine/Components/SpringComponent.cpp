#include "SpringComponent.h"

SpringComponent::SpringComponent(ObjectBase* parent, Scene* scene, const std::string& name)
    : RenderComponent(parent, scene, name)
{
    springLine = std::make_unique<Line>();
    CreateSpring();
}

void SpringComponent::Draw() const
{
    springLine->Draw();
}

void SpringComponent::SetStart(const glm::vec3& newStart)
{
    start = newStart;
    CreateSpring();
}

void SpringComponent::SetSpringLength(float newSpringLength)
{
    springLength = newSpringLength;
    CreateSpring();
}

void SpringComponent::SetDeltaSpringLength(float newDeltaSpringLength)
{
    springLength *= newDeltaSpringLength / deltaSpringLength;
    deltaSpringLength = newDeltaSpringLength;
    CreateSpring();
}

void SpringComponent::CreateSpring()
{
    springLine->ClearPoints();
    
    float radius = 1.0f;
    
    start.x = radius;
    glm::vec3 point = start;
    
    springLine->AddPoint(point);
    float omega = 0.0f;
    
    while (glm::abs(point.y - start.y) < springLength)
    {
        point.x = radius * glm::cos(omega);
        point.z = radius * glm::sin(omega);
        
        omega += 0.1f;
        
        point.y -= deltaSpringLength;
        
        springLine->AddPoint(point);
    }
}
