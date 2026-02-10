#include "CameraComponent.h"

#include "MeshComponent.h"
#include "Core/App.h"

CameraComponent::CameraComponent(ObjectBase* parent, Scene* scene, const std::string& name)
    : Component(parent, scene, name)
{
    UpdateView();
    
    auto window = App::GetCurrentWindow();
    window->onWindowSizeChangedEvent.BindRaw(this, &CameraComponent::OnWindowSizeChanged);
    UpdateProjection(window->GetBufferWidth(), window->GetBufferHeight());
}

void CameraComponent::Tick(float deltaTime)
{
    Component::Tick(deltaTime);
    
    if (followTarget)
    {
        const auto& targetPosition = followTarget->GetPosition();
        Move(targetPosition - lastTargetPosition);
        lastTargetPosition = targetPosition;
    }
}

void CameraComponent::Move(const glm::vec3& moveDelta)
{
    SetPosition(position + moveDelta);
}
    
void CameraComponent::Rotate(float pitchDelta, float yawDelta)
{
    if (lockRotation)
    {
        return;    
    }
    
    SetRotation(pitch - pitchDelta, yaw - yawDelta);
}

void CameraComponent::RotateAroundTarget(float pitchDelta, float yawDelta)
{
    if (followTarget && !lockRotation)
    {
        /*auto rotationMatrix = glm::mat4(1.0f);
        
        auto basePos = glm::vec4(position - followTarget->GetPosition(), 1.0f);
        
        rotationMatrix = glm::rotate(rotationMatrix, glm::radians(pitchDelta), glm::vec3(1.0f, 0.0f, 0.0f));
        auto relPOs = rotationMatrix * glm::vec4(position - followTarget->GetPosition(), 1.0f);
        
        float cosTheta = glm::dot(glm::normalize(basePos), glm::normalize(relPOs));
        float angleDegrees = glm::degrees(acos(cosTheta));
        
        if (!std::isnan(angleDegrees))
        {
            Rotate(angleDegrees, 0.0f);
        }
        
        basePos = relPOs;
        
        rotationMatrix = glm::rotate(rotationMatrix, glm::radians(yawDelta), glm::vec3(0.0f, 1.0f, 0.0f));

        relPOs = rotationMatrix * relPOs;
        
        cosTheta = glm::dot(glm::normalize(basePos), glm::normalize(relPOs));
        angleDegrees = glm::degrees(acos(cosTheta));
        
        if (!std::isnan(angleDegrees))
        {
            Rotate(0.0f, -angleDegrees);
        }
        
        SetPosition(followTarget->GetPosition() + glm::vec3(relPOs));*/
    }
}

void CameraComponent::SetPosition(const glm::vec3& newPosition)
{
    position = newPosition;
    UpdateView();
}

void CameraComponent::SetRotation(float newPitch, float newYaw)
{
    if (lockRotation)
    {
        return;    
    }
    
    yaw = newYaw;
    pitch = glm::clamp(newPitch, -89.9f, 89.9f);

    UpdateView();
}
    
void CameraComponent::SetAsActiveCamera()
{
    scene->SetActiveCamera(this);
}

void CameraComponent::SetFollowTarget(const Entity* target)
{
    followTarget = target;
    lastTargetPosition = target->GetPosition();
}

void CameraComponent::UpdateProjection(int bufferWidth, int bufferHeight)
{
    const auto fovy = glm::radians(perspectiveProjectionData.fov);
    const auto aspectRatio = static_cast<float>(bufferWidth) / static_cast<float>(bufferHeight);
    
    projection = glm::infinitePerspective(fovy, aspectRatio, perspectiveProjectionData.nearPlane);
}

void CameraComponent::UpdateView()
{
    const auto yawRadians = glm::radians(yaw);
    const auto pitchRadians = glm::radians(pitch);

    forward.x = sin(yawRadians);
    forward.y = sin(pitchRadians);
    forward.z = cos(yawRadians) * cos(pitchRadians);

    forward = glm::normalize(forward);

    right = glm::cross(forward, {0.0f, 1.0f, 0.0f});
    right = glm::normalize(right);
 
    up = glm::cross(right, forward);

    view = glm::lookAt(position, position + forward, up);
}

void CameraComponent::OnWindowSizeChanged(Window* window, int bufferWidth, int bufferHeight)
{
    UpdateProjection(bufferWidth, bufferHeight);
}
