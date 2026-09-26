#pragma once

#include "ObjectBase.h"

#define GENERATE_CONSTRUCTOR(ClassName, DerivedClassName) \

class SceneObject;

struct SceneObjectParams
{
    ObjectBase* parent;
    Scene* scene;
    const std::string& name;
};

struct SceneObjectHandle
{
    const Scene* scene{};
    size_t index{};
    unsigned int version{};
    
    SceneObject* Resolve() const;
};

class SceneObject : public ObjectBase
{
    friend class Scene;
public:
    SceneObject(const SceneObjectParams& params);
    
    virtual void Init() { isInitialized = true; };
    virtual void Start() {}
    virtual void PhysicsTick(float physicsDeltaTime) {}
    virtual void Tick(float deltaTime) {}
    virtual void OnDestroy();
    
    virtual void DrawUI() {}
    
    void Destroy();
    
    const SceneObjectHandle& GetHandle() const { return handle; }
    bool GetIsInitialized() const { return isInitialized; };
    
    bool tickWhenPaused{false};
    bool openUIByDefault{false};
    
protected:
    Scene* const scene;
    
private:
    SceneObjectHandle handle;
    
    bool isInitialized{false};
};
