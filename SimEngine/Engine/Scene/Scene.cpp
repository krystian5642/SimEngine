#include "Scene.h"

#include "Components/CameraComponent.h"
#include "Core/App.h"
#include "Rendering/Renderer/Renderer.h"
#include "Scene/Objects/Lighting/DirectionalLightObject.h"
#include "Scene/Objects/Lighting/PointLightObject.h"
#include "Scene/Objects/Entities/CameraEntity.h"

Scene::Scene(const std::string& name)
    : ObjectBase(nullptr, name)
{
    
}
    
void Scene::Tick(float deltaTime)
{
    static constexpr float maxDeltaTime = 1.0f / 60.0f;
    deltaTime = std::min(deltaTime, maxDeltaTime);
    
    const bool isPaused = App::currentApp->GetIsPaused();
    objects.Tick(deltaTime, isPaused);
}
    
void Scene::DestroyChild(ObjectBase* child)
{
    objects.DestroyObject(child);
}
    
void Scene::OnDestroy()
{
    objects.OnDestroy();
}
    
void Scene::Render() const
{
    Renderer::RenderSceneStatic(this);
}
    
void Scene::RegisterDirectionalLight(DirectionalLightObject* dirLight)
{
    dirLight->lightIndex = DirectionalLightObject::lightCount - 1;
    lightsData.dirLights[dirLight->lightIndex] = dirLight;
}
    
void Scene::RegisterPointLight(PointLightObject* pointLight)
{
    pointLight->lightIndex = PointLightObject::lightCount - 1;
    lightsData.pointLights[pointLight->lightIndex] = pointLight;
}
    
void Scene::UnregisterDirectionalLight(DirectionalLightObject* dirLight)
{
    lightsData.dirLights[dirLight->lightIndex] = nullptr;
}
    
void Scene::UnregisterPointLight(PointLightObject* pointLight)
{
    lightsData.pointLights[pointLight->lightIndex] = nullptr;
}
    
void Scene::RegisterRenderComponent(const RenderComponent* renderComponent)
{
    renderData.renderComponents.push_back(renderComponent);
}
    
void Scene::UnregisterRenderComponent(const RenderComponent* renderComponent)
{
    std::erase(renderData.renderComponents, renderComponent);
}
    
SceneObjectHandle Scene::RegisterObject(SceneObject* object)
{
    unsigned int version = 1;
    bool added = false;
    size_t index = 0;
    
    for (size_t i = 0; i < objectsData.objectSlots.size(); i++)
    {
        auto& slot = objectsData.objectSlots[i];
        if (slot.object == nullptr)
        {
            slot.object = object;
            version = slot.version;
            added = true;
            index = i;
            break;
        }
    }
    
    if (!added)
    {
        index = objectsData.objectSlots.size();
        const SceneObjectSlot slot{object, 1};
        objectsData.objectSlots.push_back(slot);
    }
    
    const SceneObjectHandle handle{this, index, version};
    return handle;
}
    
void Scene::UnregisterObject(SceneObject* object)
{
    for (auto& slot : objectsData.objectSlots)
    {
        if (slot.object == object)
        {
            slot.object = nullptr;
            slot.version++;
        }
    }
}
    
SceneObject* Scene::GetObjectByHandle(const SceneObjectHandle& handle) const
{
    if (handle.scene == nullptr || handle.scene != this || handle.index >= objectsData.objectSlots.size())
    {
        return nullptr;
    }
    
    const auto& slot = objectsData.objectSlots[handle.index];
    return slot.version == handle.version ? slot.object : nullptr;
}
    
glm::mat4 Scene::GetProjectionMatrix() const
{
    return activeCamera->GetProjectionMatrix();
}
    
const glm::mat4& Scene::GetViewMatrix() const
{
    return activeCamera->GetViewMatrix();
}
    
const glm::vec3& Scene::GetCameraPosition() const
{
    return activeCamera->GetPosition();
}