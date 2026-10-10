#pragma once

#include "imgui.h"

class ClassPropertyBase;

namespace UICommon
{
    bool DrawBool(const char* label, bool& value);

    bool DrawVec3(const char* label, glm::vec3& value
        , float speed, float min, float max, const char* format);

    bool DrawFloat(const char* label, float& value
        , float min, float max, const char* format);

    bool DrawInt(const char* label, int& value
        , int min, int max, const char* format);

    bool DrawColor(const char* label, glm::vec4& color);

    bool DrawDragFloat(const char* label, float& value
                       , float speed, float min, float max, const char* format);
    
    bool DrawButton(const char* label, const std::function<void()>& callback);
    
    void DrawProperties(const void* id, const std::vector<ClassPropertyBase*>& properties);
    
    void DrawLabel(const char* text);

    void DrawLabel(const char* text, const glm::vec3& color);

    void DrawLabelFormatted(const char* format, ...);
    
    template<class Container>
    void DrawObjects(const Container& objects, const std::string& defaultName = "Object")
    {
        objects.ForEach([&defaultName](auto* object, int index)
        {
            if (object->drawUI)
            {
                const std::string& name = object->GetName();
                const std::string label = name.empty() ? (defaultName + " " + std::to_string(index)) : name;

                ImGui::PushID(index);
                
                if (object->openUIByDefault)
                {
                    ImGui::SetNextItemOpen(true, ImGuiCond_Once);
                }
                
                const glm::vec3 UIColor = object->GetUIColor();
                ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(UIColor.r, UIColor.g
                    , UIColor.b, 1.0f));
                if (ImGui::TreeNode(label.c_str()))
                {
                    object->DrawUI();
                    ImGui::TreePop();
                }
                ImGui::PopStyleColor();
                
                ImGui::PopID();
            }
        });
    }
};
