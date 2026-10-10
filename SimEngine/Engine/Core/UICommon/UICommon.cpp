
#include "UICommon.h"
#include "Core/Properties/ClassProperty.h"

namespace UICommon
{
    bool DrawBool(const char* label, bool& value)
    {
        return ImGui::Checkbox(label, &value);
    }

    bool DrawVec3(const char* label, glm::vec3& value
        , float speed, float min, float max, const char* format)
    {
        return ImGui::DragFloat3(label, &value.x, speed, min, max, format);
    }

    bool DrawFloat(const char* label, float& value
        , float min, float max, const char* format)
    {
        return ImGui::SliderFloat(label, &value, min, max, format);
    }

    bool DrawInt(const char* label, int& value
        , int min, int max, const char* format)
    {
        return ImGui::SliderInt(label, &value, min, max, format);
    }
    
    bool UICommon::DrawColor(const char* label, glm::vec4& color)
    {
        return ImGui::ColorEdit4(label, &color.x);
    }

    bool UICommon::DrawDragFloat(const char* label, float& value
        , float speed, float min, float max, const char* format)
    {
        return ImGui::DragFloat(label, &value, speed, min, max, format);
    }

    bool DrawButton(const char* label, const std::function<void()>& callback)
    {
        if (ImGui::Button(label))
        {
            if (callback)
            {
                callback();
            }
            return true;
        }
        return false;
    }
    
    void DrawProperties(const void* id, const std::vector<ClassPropertyBase*>& properties)
    {
        ImGui::PushID(id);
        for (auto* property : properties)
        {
            if (!property->visible)
            {
                continue;
            }
            
            ImGui::BeginDisabled(property->readOnly);
            property->DrawUI();
            ImGui::EndDisabled();
        }
        ImGui::PopID();
    }
    
    void DrawLabel(const char* text)
    {
        ImGui::TextUnformatted(text);
    }

    void DrawLabel(const char* text, const glm::vec3& color)
    {
        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(color.r, color.g, color.b, 1.0f));
        ImGui::TextUnformatted(text);
        ImGui::PopStyleColor();
    }

    void DrawLabelFormatted(const char* format, ...)
    {
        va_list args;
        va_start(args, format);
        ImGui::TextV(format, args);
        va_end(args);
    }
}
