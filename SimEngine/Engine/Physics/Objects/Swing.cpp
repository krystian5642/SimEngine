#include "Swing.h"

#include "imgui.h"
#include "implot.h"
#include "Managers/MaterialManager.h"
#include "Managers/MeshManager.h"
#include "Components/SegmentComponent.h"
#include "Components/VectorVisualizerComponent.h"

Swing::Swing(ObjectBase* parent, Scene* scene, const std::string& name)
    : MeshEntity(parent, scene, name)
{
    meshComponent->mesh = MeshManager::Get().GetAssetByName("arrow_cylinder");
    meshComponent->material = MaterialManager::Get().GetAssetByName("chrome");
    
    meshComponent->SetScale(glm::vec3{meshLenght, 1.0f, 1.0f});
    meshComponent->Move(glm::vec3{-meshLenght / 2.0, 0.0f, 0.0f});
    
    segment = AddComponent<SegmentComponent>();
    
    attachPoint = AddComponent<MeshComponent>();
    attachPoint->mesh = MeshManager::Get().GetAssetByName("sphere");
    attachPoint->material = MaterialManager::Get().GetAssetByName("gold");
    attachPoint->SetScale(glm::vec3{0.08f});
    attachPoint->SetPosition(start);
    
    seat = AddComponent<MeshComponent>();
    seat->mesh = MeshManager::Get().GetAssetByName("sphere");
    seat->material = MaterialManager::Get().GetAssetByName("emerald");
    seat->SetScale(glm::vec3{0.15f});
    seat->SetPosition(end);
    
    segment->SetStart(start);
    segment->SetEnd(end);
    
    velocityVisualizer = AddComponent<VectorVisualizerComponent>();
    velocityVisualizer->SetStart(end);
    velocityVisualizer->SetDirection(velocity);
    velocityVisualizer->scaleLenghtFactor = 0.2f;
    velocityVisualizer->scaleFactor = 0.5f;
    velocityVisualizer->color = glm::vec4{1.0f, 1.0f, 0.0f, 0.9f};
    
    openUIByDefault = true;
}

void Swing::Tick(float deltaTime)
{
    MeshEntity::Tick(deltaTime);
    
    if (!swingImpulseApplied)
    {
        return;
    }
    
    glm::vec3 totalForce{0.0f};
    
    if (physicsData.useGravity)
    {
        const glm::vec3 gravityForce = physicsData.seatMass * glm::vec3{0.0f, physicsData.gravity, 0.0f};
        totalForce += gravityForce;
    }
    
    float lenght = glm::length(end - start);
    glm::vec3 direction = (end - start) / lenght;
    
    const float speedSquared = glm::dot(velocity, velocity);
    const glm::vec3 centripetalForce = -(physicsData.seatMass * speedSquared / lenght) * direction;
    
    const glm::vec3 tensionForce = centripetalForce
    + (physicsData.useGravity ? physicsData.seatMass * direction * physicsData.gravity * glm::dot(direction, glm::vec3{0.0f, -1.f, 0.0f}) : glm::vec3{0.0f});
    
    totalForce += tensionForce;
    
    if (physicsData.useAirResistance)
    {
        const glm::vec3 airResistanceForce = -velocity * physicsData.airResistance * deltaTime;
        totalForce += airResistanceForce;
    }
    
    const glm::vec3 acceleration = totalForce / physicsData.seatMass;
    velocity += acceleration * deltaTime;
    
    const glm::vec3 deltaMove = velocity * deltaTime;
    seat->Move(deltaMove);
    
    end = seat->GetPosition();
    
    // position correction
    lenght = glm::length(end - start);
    direction = (end - start) / lenght;
    seat->SetPosition(start + direction * physicsData.initialLenght);
    
    end = seat->GetPosition();
    segment->SetEnd(end);
    
    velocityVisualizer->SetStart(end);
    velocityVisualizer->SetDirection(velocity);
    
    runtimePlotData.Tick(deltaTime, glm::dot(velocity, glm::vec3{0.0f, 0.0f, 1.0f}));
}

void Swing::DrawUI()
{
    MeshEntity::DrawUI();
    
    if (!swingImpulseApplied && ImGui::Button("Apply Swing Impulse"))
    {
        ApplySwingImpulse();
        swingImpulseApplied = true;
    }
    
    if (!swingImpulseApplied)
    {
        const bool changed = ImGui::DragFloat("Initial Length", &physicsData.initialLenght, 0.05f, 0.0f, FLT_MAX);
        ImGui::DragFloat("Swing Impulse", &physicsData.swingImpulse, 0.05f);
        
        if (changed)
        {
            end = glm::vec3{0.0f, -physicsData.initialLenght, 0.0f};
            seat->SetPosition(end);
            segment->SetEnd(end);
        }
    }
    
    ImGui::DragFloat("Seat Mass", &physicsData.seatMass, 0.05f, 0.0f, FLT_MAX);
    
    ImGui::DragFloat("Gravity (g)", &physicsData.gravity, 0.05f);
    ImGui::DragFloat("Air Resistance", &physicsData.airResistance, 0.05f, 0.0f, FLT_MAX);

    ImGui::Checkbox("Use Gravity", &physicsData.useGravity);
    ImGui::Checkbox("Use Air Resistance", &physicsData.useAirResistance);
    
    if (ImPlot::BeginPlot("Velocity"))
    {
        ImPlot::SetupAxes("t", "v(t)", ImPlotAxisFlags_AutoFit, ImPlotAxisFlags_AutoFit);
        ImPlot::PlotLine("v(t)"
            , runtimePlotData.plot.GetXValuesData()
            , runtimePlotData.plot.GetYValuesData()
            , runtimePlotData.plot.GetPointCount());
    
        ImPlot::EndPlot();
    }
}

void Swing::ApplySwingImpulse()
{
    velocity += glm::vec3{0.0f, 0.0f, physicsData.swingImpulse} / physicsData.seatMass;
}
