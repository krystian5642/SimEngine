
#include "ClassProperty.h"
#include "Core/UICommon/UICommon.h"

void BoolProperty::DrawUI()
{
    if (UICommon::DrawBool(name.c_str(), currentValue))
    {
        InvokeOnChangedEvent();
    }
}

void Vec3Property::DrawUI()
{
    if (UICommon::DrawVec3(name.c_str(), currentValue, sliderSpeed
        , minValue, maxValue, valueFormat))
    {
        InvokeOnChangedEvent();
    }
}

void FloatProperty::DrawUI()
{
    if (UICommon::DrawFloat(name.c_str(), currentValue, minValue
        , maxValue, valueFormat))
    {
        InvokeOnChangedEvent();
    }
}

void IntProperty::DrawUI()
{
    if (UICommon::DrawInt(name.c_str(), currentValue, minValue
        , maxValue, valueFormat))
    {
        InvokeOnChangedEvent();
    }
}

void FunctionProperty::DrawUI()
{
    UICommon::DrawButton(name.c_str(), currentValue);
}