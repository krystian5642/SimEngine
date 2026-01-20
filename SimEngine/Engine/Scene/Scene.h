#pragma once

#include "Scene/Objects/Core/SceneObject.h"
#include "Scene/Objects/Core/ObjectContainer.h"
#include "Rendering/Core/Skybox.h"

class Window;
class Skybox;
class RenderComponent;
class PointLightObject;
class DirectionalLightObject;
class CameraComponent;
class Scene;
class System;
  
struct SceneLightsData
{
    static constexpr size_t maxLights{10};
    const DirectionalLightObject* dirLights[maxLights];
    const PointLightObject* pointLights[maxLights];
};

struct SceneRenderData
{
    std::vector<const RenderComponent*> renderComponents;
};

struct SceneObjectSlot
{
    SceneObject* object{};
    unsigned int version{};
};

struct SceneObjectsData
{
    std::vector<SceneObjectSlot> objectSlots;
};

class Scene : public ObjectBase
{
public:
    Scene(const std::string& name = "DefaultScene");

    virtual void Init() {}
    virtual void Start() {}
    virtual void Tick(float deltaTime);
    
    virtual void DrawImGui() const {}
    
    void DestroyChild(ObjectBase* child) override;
    
    void OnDestroy();

    void Render() const;
    
    void RegisterDirectionalLight(DirectionalLightObject* dirLight);
    void RegisterPointLight(PointLightObject* pointLight);
    void UnregisterDirectionalLight(DirectionalLightObject* dirLight);
    void UnregisterPointLight(PointLightObject* pointLight);

    void RegisterRenderComponent(const RenderComponent* renderComponent);
    void UnregisterRenderComponent(const RenderComponent* renderComponent);
    
    SceneObjectHandle RegisterObject(SceneObject* object);
    void UnregisterObject(SceneObject* object);
    SceneObject* GetObjectByHandle(const SceneObjectHandle& handle) const;
    
    glm::mat4 GetProjectionMatrix() const;
    const glm::mat4& GetViewMatrix() const;
    const glm::vec3& GetCameraPosition() const;
    
    template<class T>
    T* AddObject(const std::string& objectName = "")
    {
        return objects.AddObject<T>(this, this, objectName);
    }
    
    template<class T>
    T* GetObjectByClass()
    {
        return objects.GetObjectByClass<T>();
    }

    void SetActiveCamera(CameraComponent* camera) { activeCamera = camera; }
    const CameraComponent* GetActiveCamera() const { return activeCamera; }
    
    void SetSkybox(std::unique_ptr<Skybox> newSkybox) { skybox = std::move(newSkybox); }
    const Skybox* GetSkybox() const { return skybox.get(); }
    
    const SceneLightsData& GetLightsData() const { return lightsData; }
    const SceneRenderData& GetRenderData() const { return renderData; }
    
    size_t GetObjectCount() const { return objects.GetCount(); }

protected:
    ObjectContainer<SceneObject> objects;
    
    std::unique_ptr<Skybox> skybox;
  
private:
    SceneLightsData lightsData;
    SceneRenderData renderData;
    SceneObjectsData objectsData; // It is only used as a global handle when destroying objects
    
    CameraComponent* activeCamera{};
};
