#include "Scene.h"

#include "Rendering/Renderer/Renderer.h"
#include "Scene/Objects/Lighting/DirectionalLightObject.h"
#include "Scene/Objects/Lighting/PointLightObject.h"
#include "Scene/Objects/Entities/CameraEntity.h"

namespace SimEngine
{
    Scene::Scene(const std::string& name)
        : ObjectBase(nullptr, name)
    {
    }

    Scene::~Scene()
    {
    }

    void Scene::Init()
    {
        for (const auto& slot : objectsData.objectSlots)
        {
            slot.object->Init();
        }
    }

    void Scene::Start()
    {
        for (const auto& slot : objectsData.objectSlots)
        {
            slot.object->Start();
        }
    }

    void Scene::Tick(float deltaTime)
    {
        for (const auto& slot : objectsData.objectSlots)
        {
            slot.object->Tick(deltaTime);
        }
    }

    void Scene::OnDestroy()
    {
        for (const auto& slot : objectsData.objectSlots)
        {
            slot.object->OnDestroy();
        }
    }

    void Scene::Render(const Window& window) const
    {
        Renderer::RenderSceneStatic(this, window);
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
        SceneObjectHandle handle;
        handle.scene = this;
        handle.index = objectsData.objectSlots.size();
        handle.version = 1;
        
        SceneObjectSlot slot;
        slot.object = object;
        slot.version = 1;
        
        objectsData.objectSlots.push_back(slot);
        
        return handle;
    }

    void Scene::UnregisterObject(SceneObject* object)
    {
        std::erase_if(objectsData.objectSlots, [object](const auto& slot)
        {
            return object == slot.object;
        });
    }

    SceneObject* Scene::GetObjectByHandle(const SceneObjectHandle& handle) const
    {
        if (handle.scene != this || handle.index >= objectsData.objectSlots.size())
        {
            return nullptr;
        }
        
        return objectsData.objectSlots[handle.index].object;
    }

    void Scene::DestroyChild(ObjectBase* child)
    {
        objects.DestroyObject(child);
    }
}
