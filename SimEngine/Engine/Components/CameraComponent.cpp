#include "CameraComponent.h"

#include "MeshComponent.h"
#include "Core/App.h"

CameraComponent::CameraComponent(ObjectBase* parent, Scene* scene, const std::string& name)
    : Component(parent, scene, name)
{
    UpdateView();
    
    Window& window = App::Get().window;
    window.onWindowSizeChangedEvent.BindRaw(this, &CameraComponent::OnWindowSizeChanged);
    UpdateProjection(window.GetBufferWidth(), window.GetBufferHeight());
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

void CameraComponent::UpdateProjection(int bufferWidth, int bufferHeight)
{
    const float fovy = glm::radians(perspectiveProjectionData.fov);
    const float aspectRatio = static_cast<float>(bufferWidth) / static_cast<float>(bufferHeight);
    
    projection = glm::infinitePerspective(fovy, aspectRatio, perspectiveProjectionData.nearPlane);
}

void CameraComponent::UpdateView()
{
    const float yawRadians = glm::radians(yaw);
    const float pitchRadians = glm::radians(pitch);

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
