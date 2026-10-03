
#include "ClassProperty.h"
#include "imgui.h"

void BoolProperty::DrawUI()
{
    if (ImGui::Checkbox(name.c_str(), &currentValue))
    {
        InvokeOnChangedEvent();
    }
}

void Vec3Property::DrawUI()
{
    if (ImGui::DragFloat3(name.c_str(), &currentValue.x, sliderSpeed, minValue, maxValue, valueFormat))
    {
        InvokeOnChangedEvent();
    }
}

void FloatProperty::DrawUI()
{
    if (ImGui::SliderFloat(name.c_str(), &currentValue, minValue, maxValue, valueFormat))
    {
        InvokeOnChangedEvent();
    }
}

void FunctionProperty::DrawUI()
{
    if (ImGui::Button(name.c_str()))
    {
        currentValue();
    }
}
