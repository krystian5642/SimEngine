#pragma once

#include "Core/EventSystem/Event.h"
#include "Scene/Objects/Core/ObjectBase.h"

inline std::string CamelCaseToTitle(const std::string& input)
{
    std::string result;

    for (size_t i = 0; i < input.size(); ++i)
    {
        const unsigned char c = static_cast<unsigned char>(input[i]);

        if (i == 0)
        {
            result += static_cast<char>(std::toupper(c));
        }
        else
        {
            if (std::isupper(c))
            {
                result += ' ';
            }
            result += static_cast<char>(c);
        }
    }

    return result;
}

#define PROPERTY(Type, name, ...) \
    Type name{*this, CamelCaseToTitle(#name), __VA_ARGS__}; \

class ClassPropertyBase
{
public:
    ClassPropertyBase(ObjectBase& owner, const std::string& propertyName)
        : name(propertyName)
    {
        owner.AddProperty(this);
    }
    
    virtual ~ClassPropertyBase() = default;
   
    virtual void DrawUI() = 0;
    
    bool readOnly{false};
    bool visible{true};
    
protected:
    std::string name{};
};

template<class T>
class ClassProperty : public ClassPropertyBase
{
public:
    template <class U = T, std::enable_if_t<std::is_same_v<U, T>, int> = 0>
    explicit ClassProperty(ObjectBase& owner, const std::string& propertyName, const U& defaultValue = U{})
        : ClassPropertyBase(owner, propertyName)
        , currentValue(defaultValue)
    {
    }
    
    ClassProperty& operator=(ClassProperty const& other)
    {
        Set(other.currentValue);
        return *this;
    }
    
    ClassProperty& operator=(ClassProperty&& other) = default;
    
    ClassProperty& operator=(const T& newValue)
    {
        Set(newValue);
        return *this;
    }
    
    operator const T&() const { return currentValue; }
    
    bool operator==(const ClassProperty& other) const
    {
        return currentValue == other.currentValue;
    }
    
    template<class Obj, class Method>
    void SetOnChangedEvent(Obj* obj, Method method)
    {
        onChangedEvent = Event<const T&>(std::function<void(T)>(
            [obj, method](const T& value)
            {
                std::invoke(method, obj, value);
            }));
    }
    
    virtual void Set(const T& value)
    {
        currentValue = value;
        onChangedEvent.Invoke(currentValue);
    }
    
    const T& Get() const { return currentValue; }
    
    const std::string& GetName() const { return name; }
    
protected:
    void InvokeOnChangedEvent()
    {
        onChangedEvent.Invoke(currentValue);
    }
    
    T currentValue{};
    
private:
    Event<const T&> onChangedEvent;
};

class BoolProperty : public ClassProperty<bool>
{
public:
    using ClassProperty::ClassProperty;
    using ClassProperty::operator=;
    
    void DrawUI() override;
};

template<class T>
class NumericProperty : public ClassProperty<T>
{
public:
    NumericProperty(ObjectBase& owner, const std::string& propertyName
        , const T& defaultValue = T{}, const T& min = T(-1000)
        , const T& max = T(1000), const char* format = "%.3f")
            : ClassProperty<T>(owner, propertyName, defaultValue)
            , minValue(min)
            , maxValue(max)
            , valueFormat(format)
    {
    }
    
    using ClassProperty<T>::operator=;
    
    T minValue;
    T maxValue;
    const char* valueFormat;
};

class Vec3Property : public ClassProperty<glm::vec3>
{
public:
    Vec3Property(ObjectBase& owner, const std::string& propertyName
        , const glm::vec3& defaultValue = glm::vec3{}
        , float min = -100000.0f, float max = 100000.0f
        , float speed = 0.1f, const char* format = "%.3f")
            : ClassProperty(owner, propertyName, defaultValue)
                , minValue(min)
                , maxValue(max)
                , sliderSpeed(speed)
                , valueFormat(format)
    {
    }
    
    using ClassProperty::operator=;
    
    void DrawUI() override;
    
    const float& x = currentValue.x;
    const float& y = currentValue.y;
    const float& z = currentValue.z;
    
    float minValue;
    float maxValue;
    float sliderSpeed;
    const char* valueFormat;
};

class FloatProperty : public NumericProperty<float>
{
public:
    using NumericProperty::NumericProperty;
    using NumericProperty::operator=;
    
    void DrawUI() override;
};

class IntProperty : public NumericProperty<int>
{
public:
    IntProperty(ObjectBase& owner, const std::string& propertyName
        , int defaultValue = 0, int min = -10000.0f
        , int max = 10000.0f, const char* format = "%d")
            : NumericProperty<int>(owner, propertyName
                , defaultValue, min, max, format)
    {
    }
    
    using NumericProperty::operator=;
    
    void DrawUI() override;
};

class FunctionProperty : public ClassProperty<std::function<void()>>
{
public:
    using ClassProperty::ClassProperty;
    
    void DrawUI() override;
};















