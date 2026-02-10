#pragma once

class ObjectBase;
class Scene;

template<class ObjectBaseClass>
class ObjectContainer
{
    using ObjectBasePtr = std::unique_ptr<ObjectBaseClass>;
public:
    template <class ObjectClass>
    ObjectClass* AddObject(ObjectBase* parent, Scene* scene, bool isInitialized, const std::string& name)
    {
        auto newObject = std::make_unique<ObjectClass>(parent, scene, name);
        auto rawPtr = newObject.get();
        if (isInitialized)
        {
            objectsToAdd.push_back(std::move(newObject));
        }
        else
        {
            objects.push_back(std::move(newObject));
        }
        return rawPtr;
    }
    
    template <class ObjectClass>
    void DestroyObject(ObjectClass* object)
    {
        objectsToRemove.push_back(static_cast<ObjectBaseClass*>(object));
    }
    
    template <class ObjectClass>
    ObjectClass* GetObjectByClass() const
    {
        for (const auto& object : objects)
        {
            auto typedObject = dynamic_cast<ObjectClass*>(object.get());
            if (typedObject) 
            {
                return typedObject; 
            }
        }
        
        return nullptr;
    }
    
    template <class ObjectClass>
    void GetObjectsByClass(std::vector<ObjectClass*>& outObjects) const
    {
        for (const auto& object : objects)
        {
            auto typedObject = dynamic_cast<ObjectClass*>(object.get());
            if (typedObject) 
            {
                outObjects.push_back(typedObject);
            }
        }
    }
    
    template <class ObjectClass>
    ObjectClass* GetObjectByName(const std::string& name) const
    {
        for (const auto& object : objects)
        {
            if (object->GetName() == name)
            {
                return dynamic_cast<ObjectClass*>(object.get());
            }
        }
        
        return nullptr;
    }
    
    void ForEach(const std::function<void(ObjectBaseClass*, int)>& func) const
    {
        for (int i = 0; i < objects.size(); ++i)
        {
            if (objects[i])
            {
                func(objects[i].get(), i);
            }
        }
    }
    
    void Init()
    {
        for (const auto& object : objects)
        {
            object->Init();
        }
    }
    
    void Start()
    {
        for (const auto& object : objects)
        {
            object->Start();
        }
    }
    
    void Tick()
    {
        for (const auto& object : objectsToAdd)
        {
            object->Init();
        }
        for (const auto& object : objectsToAdd)
        {
            object->Start();
        }
        
        for (auto& object : objectsToAdd)
        {
            objects.push_back(std::move(object));
        }
        objectsToAdd.clear();
        
        for (auto* object : objectsToRemove)
        {
            object->OnDestroy();
        }
        std::erase_if(objects, [&](const ObjectBasePtr& objectPtr)
        {
            return std::find(objectsToRemove.begin(), objectsToRemove.end(), objectPtr.get()) != objectsToRemove.end();
        });
        objectsToRemove.clear();
    }
    
    void TickObjects(float deltaTime, bool isPaused, EngineTickPhase tickPhase)
    {
        for (const auto& object : objects)
        {
            if ((!isPaused || object->tickWhenPaused) && object->tickPhase == tickPhase)
            {
                object->Tick(deltaTime);
            }
        }
    }
    
    void OnDestroy()
    {
        for (const auto& object : objects)
        {
            object->OnDestroy();
        }
    }
    
    size_t GetCount() const { return objects.size(); }

private:
    std::vector<ObjectBasePtr> objects;
    std::vector<ObjectBasePtr> objectsToAdd;
    std::vector<ObjectBaseClass*> objectsToRemove;
};
