
#include "SpringComponent.h"

SpringComponent::SpringComponent(const SceneObjectParams& params)
    : RenderComponent(params)
{
    springLine = std::make_unique<Line>();
    springLine->drawMaxLenght = false;
    
    RecreateSpringPoints();
    
    coilsNum.SetOnChangedEvent(this, &SpringComponent::OnPropertyChanged<int>);
    start.SetOnChangedEvent(this, &SpringComponent::OnPropertyChanged<glm::vec3>);
    end.SetOnChangedEvent(this, &SpringComponent::OnPropertyChanged<glm::vec3>);
    radius.SetOnChangedEvent(this, &SpringComponent::OnPropertyChanged<float>);
    deltaAngle.SetOnChangedEvent(this, &SpringComponent::OnPropertyChanged<float>);
}

void SpringComponent::Draw() const
{
    springLine->Draw();
}

bool SpringComponent::IsTransparent() const
{
    return springLine->IsTransparent();
}

void SpringComponent::DrawUI()
{
    RenderComponent::DrawUI();
    springLine->DrawUI();
}

glm::vec3 SpringComponent::GetUIColor() const
{
    return glm::vec3{0.3f, 0.85f, 0.9f};
}

void SpringComponent::RecreateSpringPoints() const
{
    springLine->ClearPoints();
    
    const float deltaAngleRad = glm::radians(deltaAngle.Get());
    const glm::vec3 direction = end.Get() - start.Get();
    const float springLength = glm::length(direction);

    const glm::quat rotation = glm::rotation(glm::vec3{0.0f, 1.0f, 0.0f}, direction / springLength);
    
    const float totalAngle = glm::two_pi<float>() * static_cast<float>(coilsNum);
    const int steps = static_cast<int>(glm::ceil(totalAngle / deltaAngleRad));
    
    springLine->Reserve(steps);
    for (int i = 0; i <= steps; i++)
    {
        const float angle = glm::min(static_cast<float>(i) * deltaAngleRad, totalAngle);
        
        glm::vec3 point;
        point.x = radius * glm::cos(angle);
        point.y = (angle / totalAngle) * springLength;
        point.z = radius * glm::sin(angle);
        
        springLine->AddPoint(start.Get() + rotation * point);
    }
    
}
