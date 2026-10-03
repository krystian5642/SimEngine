#pragma once

class ClassPropertyBase;
class Scene;
class ObjectBase;

class ObjectBase
{
public:
    friend class ClassPropertyBase;
    
    ObjectBase(ObjectBase* parent, const std::string& name) : parent(parent), name(name) {}
    virtual ~ObjectBase() = 0 {}
    
    virtual void DestroyChild(ObjectBase* child) {}
    
    ObjectBase* GetParent() const { return parent; }
    void SetParent(ObjectBase* newParent) { parent = newParent; }
    
    const std::string& GetName() const { return name; }
    void SetName(std::string newName) { name = std::move(newName); }
    
    const std::vector<ClassPropertyBase*>& GetProperties() const { return properties; }

protected:
    ObjectBase* parent;
    std::string name;
    
    void AddProperty(ClassPropertyBase* newProperty)
    {
        properties.push_back(newProperty);
    }
    
    std::vector<ClassPropertyBase*> properties;
};