#pragma once

template<class T, class... Args>
class ClassProperty
{
public:
    ClassProperty(T& property);
    
    using Callback = std::function<const T&(Args...)>;
    
    virtual void DrawUI() = 0;
    
    void SetSetPropertyFunction();
    void SetGetPropertyFunction();
    
private:
    
};
