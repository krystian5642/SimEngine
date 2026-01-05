#pragma once

#include <vector>

#include "Scene/Objects/Core/SceneObject.h"
#include "Scene/Objects/Core/ObjectContainer.h"

namespace SimEngine
{
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
        ~Scene() override;
    
        virtual void Init();
        virtual void Start();
        virtual void Tick(float deltaTime);
        
        void OnDestroy();
    
        void Render(const Window& window) const;
        
        void RegisterDirectionalLight(DirectionalLightObject* dirLight);
        void RegisterPointLight(PointLightObject* pointLight);
        void UnregisterDirectionalLight(DirectionalLightObject* dirLight);
        void UnregisterPointLight(PointLightObject* pointLight);
    
        void RegisterRenderComponent(const RenderComponent* renderComponent);
        void UnregisterRenderComponent(const RenderComponent* renderComponent);
        
        SceneObjectHandle RegisterObject(SceneObject* object);
        void UnregisterObject(SceneObject* object);
        SceneObject* GetObjectByHandle(const SceneObjectHandle& handle) const;
        
        template<class T>
        T* AddObject(const std::string& objectName = "")
        {
            return objects.AddObject<T>(this, this, objectName);
        }
    
        void SetActiveCamera(CameraComponent* camera) { activeCamera = camera; }
        const CameraComponent* GetActiveCamera() const { return activeCamera; }
        
        const Skybox* GetSkybox() const { return skybox; }
        
        const SceneLightsData& GetLightsData() const { return lightsData; }
        const SceneRenderData& GetRenderData() const { return renderData; }
        
    protected:
        void DestroyChild(ObjectBase* child) override;
        
        ObjectContainer<SceneObject> objects;
  
    private:
        SceneLightsData lightsData;
        SceneRenderData renderData;
        SceneObjectsData objectsData;
        
        CameraComponent* activeCamera{};
        Skybox* skybox{};
    };
}
