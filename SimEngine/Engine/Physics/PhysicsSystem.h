#pragma once

#include "Scene/Objects/Core/SceneObject.h"

template<class TComponent, class TDerived>
class PhysicsSystem : public SceneObject
{
public:
    using SceneObject::SceneObject;
    
    static TDerived* Get() { return instance; }
    
    void Init() override;
    void OnDestroy() override;
    
    void Register(TComponent* component);
    void Unregister(TComponent* component);
    
protected:
    std::vector<TComponent*> components;
    
private:
    static inline TDerived* instance{nullptr};
};

template <class TComponent, class TDerived>
void PhysicsSystem<TComponent, TDerived>::Init()
{
    SceneObject::Init();
    
    if (instance != nullptr)
    {
        throw std::runtime_error("PhysicsSystem - instance already exists");
    }
    
    instance = static_cast<TDerived*>(this);
}

template <class TComponent, class TDerived>
void PhysicsSystem<TComponent, TDerived>::OnDestroy()
{
    SceneObject::OnDestroy();
    
    instance = nullptr;
}

template <class TComponent, class TDerived>
void PhysicsSystem<TComponent, TDerived>::Register(TComponent* component)
{
    components.push_back(component);
}

template <class TComponent, class TDerived>
void PhysicsSystem<TComponent, TDerived>::Unregister(TComponent* component)
{
    std::erase(components, component);
}