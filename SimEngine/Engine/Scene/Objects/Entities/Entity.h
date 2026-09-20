#pragma once

#include "Scene/Objects/Core/SceneObject.h"
#include "Components/SceneComponent.h"
#include "Scene/Scene.h"
#include "Scene/Objects/Core/ObjectContainer.h"

class Scene;
class ObjectBase;
class SceneComponent;
class Entity;
class Component;

class Entity : public SceneObject
{
public:
    Entity(const SceneObjectParams& params);
  
    void Init() override;
    void Start() override;
    void Tick(float deltaTime) override;
    void OnDestroy() override;
    
    void DrawUI() override;
    
    void DestroyChild(ObjectBase* child) override;
    
    void Move(const glm::vec3& moveDelta);
    void Rotate(const glm::vec3& rotateDelta);
    void Rotate(float rotationDelta, const glm::vec3& axis);
    void Scale(const glm::vec3& scaleDelta);
    
    void SetRotationMode(RotationMode newRotationMode);
    void SetPosition(const glm::vec3& newPosition);
    void SetEulerRotation(const glm::vec3& newRotation);
    void SetQuatRotation(const glm::quat& newQuatRotation);
    void SetScale(const glm::vec3& newScale);
    
    RotationMode GetRotationMode() const { return rootComponent->GetRotationMode(); }
    const glm::vec3& GetPosition() const { return rootComponent->GetPosition(); }
    const glm::vec3& GetEulerRotation() const { return rootComponent->GetEulerRotation(); }
    const glm::quat& GetQuatRotation() const { return rootComponent->GetQuatRotation(); }
    const glm::vec3& GetScale() const { return rootComponent->GetScale(); }
    
    virtual bool CanMove() const { return true; }
    
    template <class T>
    T* AddChild(const std::string& childName = "")
    {
        return childEntities.AddObject<T>(this, scene, scene->GetIsInitialized(), childName);
    }
    
    template <class T>
    T* GetChildByClass()
    {
        return childEntities.GetObjectByClass<T>();
    }
    
    template <class T>
    T* GetChildByName()
    {
        return childEntities.GetObjectByName<T>(name);
    }

    template <class T>
    T* AddComponent(const std::string& componentName = "")
    {
        auto* newComponent = components.AddObject<T>(this, scene, scene->GetIsInitialized(), componentName);
        if (rootComponent)
        {
            auto* sceneComponent = dynamic_cast<SceneComponent*>(newComponent);
            if (sceneComponent)
            {
                rootComponent->AttachComponent(sceneComponent);
            }
        }
        return newComponent;
    }
    
    template <class T>
    T* GetComponentByClass() const
    {
        return components.GetObjectByClass<T>();
    }
    
    template <class T>
    void GetComponentsByClass(std::vector<T*>& outComponents, bool includeChildren = true) const
    {
        components.GetObjectsByClass<T>(outComponents);
        
        if (includeChildren)
        {
            childEntities.ForEach([&outComponents, includeChildren](Entity* childEntity, int index)
            {
                childEntity->GetComponentsByClass<T>(outComponents, includeChildren);
            });
        }
    }
    
    template <class T>
    T* GetComponentByName(const std::string& componentName) const
    {
        return components.GetObjectByName<T>(componentName);
    }

protected:
    SceneComponent* rootComponent{};
    
    ObjectContainer<Entity> childEntities;
    ObjectContainer<Component> components;
};
